/* -*- c++ -*- */
/*
 * Copyright 2019 University of Toronto Aerospace Team.
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

/*
 * Written by Ryan Song
 */



#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "endurosat_frame_sync_bb_impl.h"
#include <iomanip>

namespace gr {
  namespace utat {

    endurosat_frame_sync_bb::sptr
    endurosat_frame_sync_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new endurosat_frame_sync_bb_impl());
    }

    /*
     * The private constructor. Define how many input and output streams there are as well as their size.
     * One stream in, one stream out, each one carries bytes
     */
    endurosat_frame_sync_bb_impl::endurosat_frame_sync_bb_impl()
      : gr::block("endurosat_frame_sync_bb",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint8_t)))
    {
        packet_training_field = 0x00000000;
        packet_data_flag = 0x00;
        packet_data_length = 0x00;
        packet_data_field.clear();
        packet_crc_checksum = 0x0000;
        state = training_field_state;
        counter = 0; 
    }

    /* Our virtual destructor */
    endurosat_frame_sync_bb_impl::~endurosat_frame_sync_bb_impl()
    {
    }

    /* lets the scheduler know the expected ratio of input to output items */
    void
    endurosat_frame_sync_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* Sync block forecast. Number of input items is equal to the number of output items */
        unsigned ninputs = ninput_items_required.size ();
        for(unsigned i = 0; i < ninputs; i++){
            ninput_items_required[i] = noutput_items;
        }
    }

    /* function that will be called to do signal processing */
    int
    endurosat_frame_sync_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        /* Cast the input and output streams */
        const uint8_t *in = (const uint8_t *) input_items[0];
        uint8_t *out = (uint8_t *) output_items[0];

        /* Number of input items equal number of output items */
        const int ninput = noutput_items;

        std::vector<uint8_t> processed;
        /* Iterate through this buffer */
        for(int i=0; i<ninput; i++){
            /* send this byte in for processing */
            processed = process_byte( in[i] );
            /* send the exact same information down the stream without alteration */
            // XXX Consider modifying the actual byte output here - send 0x1D for non-packets
            if (processed.empty()) {
                out[i] = 0;
            } else {
                for (int j = 0; j < processed.size(); j++) {
                    out[i-processed.size()+1+j] = processed[j];
                }
            }
        }

        /* Tell runtime system how many input items we consumed on input stream. */
        consume_each (ninput);

        // XXX Alternatively, update the output here, not in the loop

        /* Tell runtime system how many output items we produced. */
        return noutput_items;
    }

    /* function that processes the byte */
    std::vector<uint8_t>
    endurosat_frame_sync_bb_impl::process_byte( uint8_t byte ) {
        /* depending on the state, handle the byte differently */
        switch (state) {
            case training_field_state: 
                /* read value of byte and push onto shift register */
                if(byte == 0x01) 
                    packet_training_field = (packet_training_field << 1) + 1;
                else if(byte == 0x00) 
                    packet_training_field = packet_training_field << 1;
                
                /* check if the training field has been reached, if so update the state */
                if(packet_training_field == 0x55555555) {
                    // std::cout << "Training field detected" << '\n';
                    state = data_flag_state;
                }
                
                break;

            case data_flag_state:
                /* use counter to keep track of position. Flag must be found withing 16 bits */
                if(counter < 32) {
                    /* read value of byte and push onto shift register */
                    if(byte == 0x01) 
                        packet_data_flag = (packet_data_flag << 1) + 1;
                    else if(byte == 0x00) 
                        packet_data_flag = packet_data_flag << 1;

                    /* increment */
                    counter++;

                    if(packet_data_flag == 0x7E) {
                        // std::cout << "Data flag detected" << '\n';
                        counter = 0;
                        state = data_length_state;
                    }
                }
                else {
                    clear_all_packet_reg();
                }
                
                break;

            case data_length_state:
                /* read value of byte and push onto shift register for 8 positions */
                if(counter < 8) {
                    if(byte == 0x01) 
                        packet_data_length = (packet_data_length << 1) + 1;
                    else if(byte == 0x00) 
                        packet_data_length = packet_data_length << 1;
                    /* increment */
                    counter++;

                    if(counter == 8) {
                        // std::cout << "Data length is: " << (int) packet_data_length << '\n'; 
                        counter = 0;
                        state = data_field_state;
                    }
                } 
            
                break;

            case data_field_state:
                /* 8*packet_data_length positions should be read */
                if(counter < 8*packet_data_length) {
                    /* if counter is a multiple of 8, it means there is a new byte */
                    if((counter % 8) == 0) {
                        packet_data_field.push_back(0x00);
                    } 
                    int byte_num = (int)(counter / 8);
                    /* read value of new byte */
                    if(byte == 0x01) 
                        packet_data_field[byte_num] = (packet_data_field[byte_num] << 1) + 1;
                    else if(byte == 0x00) 
                        packet_data_field[byte_num] = packet_data_field[byte_num] << 1;
                    /* increment */
                    counter++;

                    if(counter == 8*packet_data_length){
                        // std::cout << "Data field received " << '\n';
                        counter = 0;
                        state = crc_checksum_state;
                    }
                }
                
                break;

            case crc_checksum_state:
                /* read value of byte and push onto shift register for 8 positions */
                if(counter < 16) {
                    if(byte == 0x01) 
                        packet_crc_checksum = (packet_crc_checksum << 1) + 1;
                    else if(byte == 0x00) 
                        packet_crc_checksum = packet_crc_checksum << 1;
                    /* increment */
                    counter++;

                    if (counter == 16) {
                        std::cout << std::endl << std::endl << "==========================" << std::endl;
                        // std::cout << "CRC checksum is: " << packet_crc_checksum << '\n'; 
                        if( crc16(packet_data_field, packet_data_length) == packet_crc_checksum ) {
                        /* IF YOU GET HERE - YOU HAVE RECEIVED A VALID ENDUROSAT PACKET. */
                            std::cout << "PACKET RECEIVED! " << std::endl;
                            std::cout << "Size: " << packet_data_field.size() << std::endl;
                            std::cout << "Contents (HEX): " << std::endl;
                            for(int i=0; i<packet_data_field.size(); i++) {
                                std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << int(packet_data_field[i]) << " ";
                            }
                            std::cout << std::dec << std::endl;
                            std::cout << "Contents (text): " << std::endl;
                            for (int i = 0; i < packet_data_field.size(); i++) {   
                                std::cout << (char)packet_data_field[i];
                            }
                            std::cout << std::endl;
                        /* YOU SHOULD NOW EXTRACT THE RELEVANT INFORMATION FROM THE UTAT FRAME WITHIN THAT */
                            
                            // Get length
                            uint8_t dec_length = packet_data_field[1];

                            // Get command ID & status
                            // NB: Decoded message is from [3] to [3+length-1] so use [3+i]
                            uint16_t cmd_id = (((uint16_t)packet_data_field[3] << 8) | (uint16_t)packet_data_field[4]);
                            uint8_t status = packet_data_field[5];
                            std::vector<uint8_t> data;

                            // Check if it's a N/ACK or response
                            if ((packet_data_field[3] & (0x1 << 7)) == 0x0) {
                                // MSB is 0: is an ACK

                            } else {

                            }
                            for (int i = 0; i < dec_length; i++) {

                            }


                        } 
                        else {
                            std::cerr << "PACKET PROCESSED: BAD CRC" << std::endl;
                        }
                        std::cout << "==========================" << std::endl;

                        // Clean up
                        std::vector<uint8_t> output (packet_data_field);
                        clear_all_packet_reg();
                        counter = 0;
                        state = training_field_state;
                        return output;
                    }
                } 
                
                break;

            default: 
                clear_all_packet_reg(); 
                state = training_field_state;
                break;

        }
        return std::vector<uint8_t>();
    }

    /* clears all packet registers */
    void 
    endurosat_frame_sync_bb_impl::clear_all_packet_reg() {
        packet_training_field = 0x00000000;
        packet_data_flag = 0x00;
        packet_data_length = 0x00;
        packet_data_field.clear();
        packet_crc_checksum = 0x0000;
        state = training_field_state;
        counter = 0; 
    }

    /* bad_crc16-ccitt value is calculated on the data length and the data field concatenated into one string of bytes */
    uint16_t
    endurosat_frame_sync_bb_impl::crc16(const std::vector<uint8_t> &data_field, uint8_t data_length) {
        uint16_t poly = 0x1021;
        uint16_t data_byte = 0x0000;
        uint16_t bad_crc = 0xFFFF;

        /* calculate value with data_length as input */
        data_byte = data_length;
        data_byte <<= 8;
        for(int i=0; i<8; i++) {
            uint16_t xor_flag = (bad_crc ^ data_byte) & 0x8000;
            bad_crc <<= 1;
            if(xor_flag) {
                bad_crc ^= poly;
            }
            data_byte <<= 1;
        }

        /* calculate crc value for the rest of the datafields */
        for(int i=0; i<data_field.size(); i++){
            data_byte = data_field[i];
            data_byte <<= 8;
            for(int i=0; i<8; i++) {
                uint16_t xor_flag = (bad_crc ^ data_byte) & 0x8000;
                bad_crc <<= 1;
                if(xor_flag) {
                    bad_crc ^= poly;
                }
                data_byte <<= 1;
            }
        }

        /* return crc_value */
        return bad_crc;
    } 

  } /* namespace utat */
} /* namespace gr */
