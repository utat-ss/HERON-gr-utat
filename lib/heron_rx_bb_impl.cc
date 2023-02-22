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
    DEBUG_STREAM("running destructor\n");
}

void heron_rx_bb_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    DEBUG_STREAM("running forecast\n");
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

    DEBUG_STREAM("running general work with noutput_items = " << noutput_items << '\n');

    for (int i = 0; i < noutput_items; i++) {
        // Assume will be consuming noutput_items (full size), and leave if we find a packet
        process_byte ( in[i] );
        if (d_state == finished_packet) {
            if (crc16(d_pkt.data, d_pkt.size_byte) == d_pkt.checksum) {
                // 1a) IF CHECKSUM VALID, set up output (out[] and output_size)
                DEBUG_STREAM("PACKET RECEIVED - VALID CHECKSUM\n");
                output_size = (int)d_pkt.size_byte + 1;
                for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                    out[j] = d_pkt.data[j];
                }
                out[d_pkt.size_byte] = 0x0D; // concatenate with a Carriage Return
            } else {
                // 1b) IF CHECKSUM INVALID, print error message, flush out[]
                DEBUG_STREAM("PACKET FAILED - INVALID CHECKSUM\n");
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
                debug_file << d_pkt.data[j] << " ";
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

        DEBUG_STREAM("did not get a packet in general_work\n");
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

            DEBUG_STREAM("getting preamble\n");

            d_pkt.append_bit_to_preamble(bit_byte);
            if (d_pkt.preamble_identified()) {
                d_state = getting_sync_word;
                DEBUG_STREAM(" ::: Finished getting preamble ::: " << std::endl);
            }
            break;

        case getting_sync_word:

            DEBUG_STREAM("getting sync word\n");

            d_pkt.append_bit_to_sync_word(bit_byte);
            if(d_pkt.sync_word_identified()){
                d_pkt.counter = 0;
                d_state = getting_size_byte;
                DEBUG_STREAM(" ::: Finished getting sync word ::: " << std::endl);
            }else if(d_pkt.sync_word_timeout()){
                clear_packet();
            }
            break;

        case getting_size_byte:
            DEBUG_STREAM("getting size byte\n");

            d_pkt.append_bit_to_size_byte(bit_byte);
            if(d_pkt.size_byte_identified()){
                d_pkt.counter = 0;
                d_state = getting_data;
                DEBUG_STREAM(" ::: Finished getting size byte: " << std::to_string(d_pkt.size_byte) << " bytes expected ::: " << std::endl);
            }
            break;

        case getting_data:

            DEBUG_STREAM("getting data\n");

            d_pkt.append_bit_to_data(bit_byte);
            if(d_pkt.data_identified()){
                d_pkt.counter = 0;
                d_state = getting_checksum;
                DEBUG_STREAM(" ::: Finished getting data ::: " << std::endl);
            }
            break;

        case getting_checksum:
        
            DEBUG_STREAM("getting checksum\n");

            d_pkt.append_bit_to_checksum(bit_byte);
            if(d_pkt.checksum_identified()){
                d_pkt.counter = 0;
                d_state = finished_packet;
                DEBUG_STREAM(" ::: Finished getting checksum ::: " << std::endl);
            }
            break;

        default:
            DEBUG_STREAM("defaulted out\n");

            clear_packet();
            d_state = getting_preamble;
            break;
    }

}

void
heron_rx_bb_impl::clear_packet(){
    d_state = getting_preamble;
    d_pkt.clear();
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
