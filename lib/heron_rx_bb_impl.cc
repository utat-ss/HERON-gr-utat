/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "heron_rx_bb_impl.h"
#include <gnuradio/io_signature.h>
#include <iomanip>

namespace gr {
namespace UTAT_HERON {

using input_type = uint8_t;
using output_type = uint8_t;

heron_rx_bb::sptr heron_rx_bb::make()
{
    return gnuradio::make_block_sptr<heron_rx_bb_impl>();
}

heron_rx_bb_impl::heron_rx_bb_impl()
    : gr::block(
        "heron_rx_bb",
        gr::io_signature::make(1, 1, sizeof(input_type)),
        gr::io_signature::make(1, 1, sizeof(output_type)))
{
    #ifdef DEBUG_FILE
    debug_file.open(DEBUG_FILE, std::ios_base::out);
    debug_file << "starting debug at time = " << std::time(0) << '\n';
    debug_file << "running constructor\n";
    std::cout << "running constructor, debug: " << DEBUG_FILE << '\n';
    #endif

    clear_packet();
}

heron_rx_bb_impl::~heron_rx_bb_impl(){
    #ifdef DEBUG_FILE
    debug_file << "running destructor\n";
    #endif
}

void heron_rx_bb_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    #ifdef DEBUG_FILE
    debug_file << "running forecast\n";
    #endif
    ninput_items_required[0] = noutput_items;
}

int heron_rx_bb_impl::general_work(int noutput_items,
                                   gr_vector_int& ninput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    int num_bytes_processed = 0;
    int output_size = 0;

    #ifdef DEBUG_FILE
    debug_file << "running general work with noutput_items = " << noutput_items << "\n";
    #endif

    for (int i = 0; i < noutput_items; i++) {
        // Assume will be consuming noutput_items (full size), and leave if we find a packet
        process_byte ( in[i] );
        if (d_state == finished_packet) {
            if (crc16(d_pkt.data, d_pkt.size_byte) == d_pkt.checksum) {
                // 1a) IF CHECKSUM VALID, set up output (out[] and output_size)
                #ifdef DEBUG_FILE
                debug_file << "PACKET RECEIVED - VALID CHECKSUM" << std::endl;
                #endif
                output_size = (int)d_pkt.size_byte + 1;
                for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                    out[j] = d_pkt.data[j];
                }
                out[d_pkt.size_byte] = 0x0D; // concatenate with a Carriage Return
            } else {
                // 1b) IF CHECKSUM INVALID, print error message, flush out[]
                #ifdef DEBUG_FILE
                debug_file << "PACKET FAILED - INVALID CHECKSUM" << std::endl;
                #endif
                output_size = 1;
                out[0] = 0x0D; // send a carriage return
            }
            // 2) Print packet content as HEX and TEXT 
            #ifdef DEBUG_FILE
            debug_file << "Packet length: " << std::to_string(d_pkt.size_byte) << std::endl;
            debug_file << "Packet contents (HEX): " << std::endl;
            for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                debug_file << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)d_pkt.data[j] << " ";
            }
            debug_file << std::endl;
            debug_file << std::dec;
            debug_file << "Packet contents (regular text): " << std::endl;
            for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                debug_file << "0x" << std::to_string(d_pkt.data[j]) << " ";
            }
            debug_file << std::endl;
            debug_file << "================================" << std::endl;
            #endif
            // 3. Clean up "register" values and exit
            clear_packet();
            num_bytes_processed = i;
            break;
        }

    }
    
    if (num_bytes_processed != 0) {
        // IF FOUND A PACKET, FAILED OR NOT:

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (num_bytes_processed);

        // Tell runtime system how many output items we produced.
        return output_size;
    } else {
        // IF DID NOT GET PACKET
        // TODO: Update this to output something, right now it just outputs 0.
        //for (int i = 0; i < noutput_items; i++) {
        //    out[i] = in[i];
        //}
        consume_each (noutput_items);

        #ifdef DEBUG_FILE
        debug_file << "did not get a packet in general_work\n";
        #endif
        // return noutput_items;
        return 0;
    }

}

void
heron_rx_bb_impl::process_byte (uint8_t bit_byte)
{

    // set baesfield format to hex, for d_pkt.preamble
    #ifdef DEBUG_FILE
    std::hex;
    char buf[1000];
    snprintf(buf, 1000, "entering process_byte with d_pkt.preamble = %#X\n", d_pkt.preamble);
    debug_file << buf;
    #endif

    // This block goes before a Pack K(8) Bits block because we need flexible alignment - it will pack them on the output
    switch (d_state) {
        case getting_preamble:
            #ifdef DEBUG_FILE
            debug_file << "getting preamble\n";
            #endif
            // Update stored preamble
            append_bit(bit_byte, d_pkt.preamble);
            // Once you get 31b or 32b of preamble, update state and get the sync word.
            if ((d_pkt.preamble & 0xFFFFFFFF) == 0xAAAAAAAA) {
                d_state = getting_sync_word;
                #ifdef DEBUG_FILE
                debug_file << " ::: Finished getting preamble ::: " << std::endl;
                #endif
            }
            break;

        case getting_sync_word:
            #ifdef DEBUG_FILE
            debug_file << "getting sync word\n";
            #endif

            // No way to know if preamble ended aligned w previous step - use d_counter to quit if you don't find 0x7E soon
            if (d_counter < 50) { // 32 or 33 should be sufficient, but 50 allows for some coincidental 010101 pattern before the real preamble
                append_bit(bit_byte, d_pkt.sync_word);
                d_counter++;
                // Once you get the right sync word, update the state and move to get the byte size.
                if ((d_pkt.sync_word & 0xFF) == 0x7E) {
                    d_counter = 0;
                    d_state = getting_size_byte;
                    #ifdef DEBUG_FILE
                    debug_file << " ::: Finished getting sync word ::: " << std::endl;
                    #endif
                }
            } else {
                clear_packet(); // this resets the state and sets counter to 0
            }
            break;

        case getting_size_byte:
            #ifdef DEBUG_FILE
            debug_file << "getting size byte\n";
            #endif
            // Now have fixed-size fields, so use counter for that
            if (d_counter < 8) {
                append_bit(bit_byte, d_pkt.size_byte);
                d_counter++;
                // Once you've read in the fixed byte size field, print the size out and update the state.
                if (d_counter == 8) {
                    d_counter = 0;
                    d_state = getting_data;
                    #ifdef DEBUG_FILE
                    debug_file << " ::: Finished getting size byte: " << std::to_string(d_pkt.size_byte) << " bytes expected ::: " << std::endl;
                    #endif
                }
            }
            break;

        case getting_data:
            #ifdef DEBUG_FILE
            debug_file << "getting data\n";
            #endif

            // Use d_pkt.size_byte to get fixed size of the data field
            //   e.g. if d_pkt.size_byte == 0x0A, should be reading 10 bytes == 80 "bit bytes"
            if (d_counter < 8*d_pkt.size_byte) { // -- will always end on counter % 8 == 7, which is good
                // If counter is a multiple of 8 you add a new byte
                if (d_counter % 8 == 0) {
                    d_pkt.data.push_back(0x00);
                }
                // Write to that new byte as you go
                int byte_idx = (int)(d_counter/8); // Floor counter/8
                append_bit(bit_byte, d_pkt.data[byte_idx]);
                d_counter++;
                // Once the data has been read out, print that the process is done, then move to checksum. 
                if (d_counter == 8*d_pkt.size_byte) {
                    d_counter = 0;
                    d_state = getting_checksum;
                    #ifdef DEBUG_FILE
                    debug_file << " ::: Finished getting data ::: " << std::endl;
                    #endif
                }
            }
            break;

        case getting_checksum:
            #ifdef DEBUG_FILE
            debug_file << "getting checksum\n";
            #endif
            if (d_counter < 16) {
                append_bit(bit_byte, d_pkt.checksum);
                d_counter++;
                // Check if have full checksum
                if (d_counter == 16) {
                    d_state = finished_packet;
                    #ifdef DEBUG_FILE
                    debug_file << " ::: Finished getting checksum ::: " << std::endl;
                    #endif
                }
            }
            break;

        default:
            #ifdef DEBUG_FILE
            debug_file << "defaulted out\n";
            #endif

            clear_packet();
            d_state = getting_preamble;
            break;
    }

}

template<typename T> void
heron_rx_bb_impl::append_bit(uint8_t bit, T& data){
    if (bit == 0x01) {
        data = (data << 1) + 1;
    } else if (bit == 0x00) {
        data = data << 1;
    }else{
        #ifdef DEBUG_FILE
        debug_file << "!!! Don't use Pack K Bits before this block, must be 0x00 or 0x01 input !!!" << std::endl;
        #endif
    }
}

void
heron_rx_bb_impl::clear_packet(){
    d_state = getting_preamble;
    d_pkt.preamble = 0x00000000;
    d_pkt.sync_word = 0x00;
    d_pkt.size_byte = 0x00;
    d_pkt.data.clear();
    d_pkt.checksum = 0x0000;
    d_counter = 0;
}

uint16_t
heron_rx_bb_impl::crc16(const std::deque<uint8_t> &data, uint8_t data_length)
{
    // Uses the "CRC16/CCITT-FALSE implementation from https://crccalc.com
    uint16_t poly = 0x1021;
    uint16_t data_byte = 0x0000;
    uint16_t crc = 0xFFFF;

    /* calculate value with data_length as input */
    data_byte = data_length;
    data_byte <<= 8;
    for(int i=0; i<8; i++) {
        uint16_t xor_flag = (crc ^ data_byte) & 0x8000;
        crc <<= 1;
        if(xor_flag) {
            crc ^= poly;
        }
        data_byte <<= 1;
    }

    /* calculate crc value for the rest of the datafields */
    for(int i=0; i<data.size(); i++){
        data_byte = data[i];
        data_byte <<= 8;
        for(int i=0; i<8; i++) {
            uint16_t xor_flag = (crc ^ data_byte) & 0x8000;
            crc <<= 1;
            if(xor_flag) {
                crc ^= poly;
            }
            data_byte <<= 1;
        }
    }

    /* return crc_value */
    return crc;
}

}
}
