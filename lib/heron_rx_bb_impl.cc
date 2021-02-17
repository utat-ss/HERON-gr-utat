/* -*- c++ -*- */
/*
 * Copyright 2020 University of Toronto Aerospace Team.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "heron_rx_bb_impl.h"

namespace gr {
  namespace utat {

    heron_rx_bb::sptr
    heron_rx_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new heron_rx_bb_impl());
    }


    /*
     * The private constructor
     */
    heron_rx_bb_impl::heron_rx_bb_impl()
      : gr::block("heron_rx_bb",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint8_t)))
    {
        d_state = getting_preamble;
        d_pkt.preamble = 0x00000000;
        d_pkt.sync_word = 0x00;
        d_pkt.size_byte = 0x00;
        d_pkt.data.clear();
        d_pkt.checksum = 0x0000;
        d_counter = 0;
    }

    /*
     * Our virtual destructor.
     */
    heron_rx_bb_impl::~heron_rx_bb_impl()
    {
    }

    void
    heron_rx_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    heron_rx_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const uint8_t *in = (const uint8_t *) input_items[0];
        uint8_t *out = (uint8_t *) output_items[0];

        // Do <+signal processin/g+>
        int num_bytes_processed = 0;
        int output_size = 0;

        for (int i = 0; i < noutput_items; i++) {
            // Assume will be consuming noutput_items (full size)
            // Will jack out early if we find a packet
            process_byte ( in[i] );
            if (d_state == finished_packet) {
                if (crc16(d_pkt.data, d_pkt.size_byte) == d_pkt.checksum) {
                    // 1a. IF CHECKSUM VALID, set up output (out[] and output_size)
                    std::cout << "PACKET RECEIVED - VALID CHECKSUM" << std::endl;
                    output_size = (int)d_pkt.size_byte + 1;
                    for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                        out[j] = d_pkt.data[j];
                    }
                    out[d_pkt.size_byte] = 0x0D; // concatenate with a Carriage Return
                } else {
                    // 1b. IF CHECKSUM INVALID, print error message, flush out[]
                    std::cout << "PACKET FAILED - INVALID CHECKSUM" << std::endl;
                    output_size = 1;
                    out[0] = 0x1D; // send a carriage return just in case
                }
                // 2. Print packet content as HEX and TEXT 
                std::cout << "Packet length: " << d_pkt.size_byte << std::endl;
                std::cout << "Packet contents (HEX): " << std::endl;
                for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)d_pkt.data[j] << " ";
                }
                std::cout << std::endl;
                std::cout << std::dec;
                std::cout << "Packet contents (regular text): " << std::endl;
                for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                    std::cout << (char)d_pkt.data[j] << " ";
                }
                std::cout << std::endl;
                std::cout << "================================" << std::endl;
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
            // TODO: dvogel: Can set to output nothing, should change this once reliable
            for (int i = 0; i < noutput_items; i++) {
                out[i] = in[i];
            }
            consume_each (noutput_items);
            return noutput_items;
        }
    }

    void
    heron_rx_bb_impl::process_byte (uint8_t bit_byte)
    {
        // This block goes before a Pack K(8) Bits block because we need flexible alignment - it will pack them on the output
        //   TODO add a check for if bit_byte isn't 0x00 or 0x01 - raise exception and report that you shouldn't use Pack K Bits before this block
        switch (d_state) {
            case getting_preamble:
                // Update stored preamble
                if (bit_byte == 0x01) {
                    d_pkt.preamble = (d_pkt.preamble << 1) + 1;
                } else if (bit_byte == 0x00) {
                    d_pkt.preamble = d_pkt.preamble << 1;
                }                
                // Check if you've got 31 or 32b of preamble - in which case, move on
                if (d_pkt.preamble == 0x555555) {
                    d_state = getting_sync_word;
                    std::cout << " ::: Finished getting preamble ::: " << std::endl;
                }
                break;

            case getting_sync_word:
                // No way to know if preamble ended aligned w previous step - use d_counter to quit if you don't find 0x7E soon
                if (d_counter < 50) { // quasi-arbitrary : 32 or 33 should be sufficient, but 50 allows for some coincidental 010101 pattern before the real preamble
                    if (bit_byte == 0x01) {
                        d_pkt.sync_word = (d_pkt.sync_word << 1) + 1;
                    } else if (bit_byte == 0x00) {
                        d_pkt.sync_word = d_pkt.sync_word << 1;
                    }
                    d_counter++;
                    // Check if you've got the right pattern - in which case, move on
                    if (d_pkt.sync_word == 0x7E) {
                        d_counter = 0;
                        d_state = getting_size_byte;
                        std::cout << " ::: Finished getting sync word ::: " << std::endl;
                    }
                } else {
                    clear_packet(); // this resets the state and sets counter to 0
                }
                break;

            case getting_size_byte:
                // Now have fixed-size fields, so use counter for that
                if (d_counter < 8) {
                    if (bit_byte == 0x01) {
                        d_pkt.size_byte = (d_pkt.size_byte << 1) + 1;
                    } else if (bit_byte == 0x00) {
                        d_pkt.size_byte = d_pkt.size_byte << 1;
                    }
                    d_counter++;
                    // Check if you're finished - if so, prep to move forward
                    if (d_counter == 8) {
                        d_counter = 0;
                        d_state = getting_data;
                        std::cout << " ::: Finished getting size byte: " << d_pkt.size_byte << " bytes expected ::: " << std::endl;
                    }
                }
                break;

            case getting_data:
                // Use d_pkt.size_byte to get fixed size of the data field
                //   e.g. if d_pkt.size_byte == 0x0A, should be reading 10 bytes == 80 "bit bytes"
                if (d_counter < 8*d_pkt.size_byte) { // -- will always end on counter % 8 == 7, which is good
                    // If counter is a multiple of 8 - have a new byte. Push on as 0x00
                    if (d_counter % 8 == 0) {
                        d_pkt.data.push_back(0x00);
                    }
                    // Write to that new byte as you go
                    int byte_idx = (int)(d_counter/8); // Floor counter/8
                    if (bit_byte == 0x01) {
                        d_pkt.data[byte_idx] = (d_pkt.data[byte_idx] << 1) + 1;
                    } else if (bit_byte == 0x00) {
                        d_pkt.data[byte_idx] = d_pkt.data[byte_idx] << 1;
                    }
                    d_counter++;
                    // Check if you're finished - if so, move on
                    if (d_counter == 8*d_pkt.size_byte) {
                        d_counter = 0;
                        d_state = getting_checksum;
                        std::cout << " ::: Finished getting data ::: " << std::endl;
                    }
                }
                break;

            case getting_checksum:
                if (d_counter < 16) {
                    if (bit_byte == 0x01) {
                        d_pkt.checksum = (d_pkt.checksum << 1) + 1;
                    } else if (bit_byte == 0x00) {
                        d_pkt.checksum = d_pkt.checksum << 1;
                    }
                    d_counter++;

                    // Check if have full checksum
                    if (d_counter == 16) {
                        d_state = finished_packet;
                        std::cout << " ::: Finished getting checksum ::: " << std::endl;
                    }
                }
                break;


            default:
                clear_packet();
                d_state = getting_preamble;
                break;
        }
    }

    void
    heron_rx_bb_impl::clear_packet()
    {
        d_state = getting_preamble;
        d_pkt.preamble = 0x00000000;
        d_pkt.sync_word = 0x00;
        d_pkt.size_byte = 0x00;
        d_pkt.data.clear();
        d_pkt.checksum = 0x0000;
        d_counter = 0;
    }

    /* CRC16 value is calculated on the data length and the data field concatenated into one string of bytes */
    uint16_t
    heron_rx_bb_impl::crc16(const std::vector<uint8_t> &data, uint8_t data_length)
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

  } /* namespace utat */
} /* namespace gr */
