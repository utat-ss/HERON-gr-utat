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
#include <iostream>
#include <vector>

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
    {}

    /*
     * Our virtual destructor.
     */
    endurosat_frame_sync_bb_impl::~endurosat_frame_sync_bb_impl()
    {
    }

    void
    endurosat_frame_sync_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        // This parameter tells the scheduler the preferred length of buffer
        unsigned ninputs = ninput_items_required.size ();
        for(unsigned i = 0; i < ninputs; i++){
            ninput_items_required[i] = noutput_items*REQUESTED_BUFFER_RATIO;
            //ninput_items_required[i] = REQUESTED_BUFFER_LENGTH;
        }
    }

    int
    endurosat_frame_sync_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        /* Input and output streams and their length */
        const uint8_t *in = (const uint8_t *) input_items[0];
        uint8_t *out = (uint8_t *) output_items[0];
        const int ninput = noutput_items*REQUESTED_BUFFER_RATIO;

        /* Shift registers, data vector, and stream iterator */
        uint32_t training_field = 0x00000000;
        uint8_t data_flag = 0x00;
        std::vector<std::vector<uint8_t> > data;
        uint32_t stream_it = 0;

        std::cout << ninput << '\n';

        /* Iterate over input stream */
        while(stream_it < ninput){    

            /* Parse stream and store values into training field */
            if(in[stream_it] == 0x01) 
                training_field = (training_field << 1) + 1;
            else if(in[stream_it] == 0x00) 
                training_field = training_field << 1;

            stream_it++;

            // std::cout << training_field << '\n';

            /* If the training field exists, then continue to iterate down the stream until the data flag */
            if(training_field == 0x55555555) {
                std::cout << "Training field detected " << '\n';

                /* Look for data flag (0x7E) within the next 32 bits */
                bool data_flag_found = false;

                for(int i=0; i<32; i++) {
                    if(!(stream_it<ninput)) {
                        std::cerr << " Reached end of buffer when searching for data flag " << '\n';
                        break;
                    }

                    /* Parse stream and store values into data flag */
                    if(in[stream_it] == 0x01) 
                        data_flag = (data_flag << 1) + 1;
                    else if(in[stream_it] == 0x00) 
                        data_flag = data_flag << 1;

                    stream_it++;

                    /* If flag is found, process the packet */
                    if(data_flag == 0x7E) {
                        data_flag_found = true;
                        std::cout << "Data flag detected " << '\n';
                        
                        uint8_t data_length = 0x00;
                        uint16_t crc_check = 0x0000;
                        std::vector<uint8_t> data_field;

                        /* Move along the stream and check for data length */
                        if(stream_it + 8 < ninput){
                            for(int i=0; i<8; i++){
                                if(in[stream_it+i] == 0x01)
                                    data_length = (data_length << 1) + 1;
                                else if(in[stream_it+i] == 0x00)
                                    data_length = data_length << 1;
                            }
                            // Increment
                            stream_it += 8;
                        } else {
                            std::cerr << " Reached end of buffer when reading data length " << '\n';
                            break;
                        }

                        std::cout << "Data length: " << data_length << '\n';

                        /* 
                         * Use length of buffer to search for the data field
                         * length of data in the stream is the length of the data muliplied by 8
                         */
                        if(stream_it + data_length*8 < ninput){
                            /* Iterate over the bytes as well as the individual bits */
                            for(int byte_num=0; byte_num<data_length; byte_num++){
                                uint8_t data_byte=0x00;
                                for(int bit_num=0; bit_num<8; bit_num++){
                                    if(in[stream_it + 8*byte_num + bit_num] == 0x01){
                                        data_byte = (data_byte << 1) + 1;
                                    } else if (in[stream_it + 8*byte_num + bit_num] == 0x00) {
                                        data_byte = data_byte << 1;
                                    }
                                }
                                data_field.push_back(data_byte);
                            }
                            // Increment
                            stream_it += data_length*8;
                        } else {
                            std::cerr << " Reached end of buffer when reading data " << '\n';
                            break;
                        }

                        /* Move along the stream and check for crc16-citt checksum */
                        if(stream_it + 16 < ninput){
                            for(int i=0; i<16; i++){
                                if(in[stream_it+i] == 0x01)
                                    crc_check = (crc_check << 1) + 1;
                                else if(in[stream_it+i] == 0x00)
                                    crc_check = crc_check << 1;
                            }
                            // Increment
                            stream_it += 16;
                        } else {
                            std::cerr << " Reached end of buffer when reading crc16 field " << '\n';
                            break;
                        }

                        std::cout << "CRC field: " << crc_check << '\n';

                        // Perform checkssum to see if packet is valid
                        if( crc16(data_field, data_length) == crc_check ) {
                            /* push data_field onto the main data vector to be returned */
                            data.push_back(data_field);
                        } else {
                            std::cerr << " BAD CRC " << '\n';
                        }

                        break;
                    }
 
                }   

                if(!data_flag_found){
                    std::cerr << " Did not find data flag " << '\n';
                } 
                /* Reset shift registers for the next potential packet */
                training_field = 0x00000000;
                data_flag = 0x00;
            }
        }

        /* Send processed packet data to std::out */
        for ( int packet_it=0; packet_it<data.size(); packet_it++ ){
            std::cout << "Packet Data: ";
            for( int byte_it=0; byte_it<data[packet_it].size(); byte_it++ ){
                std::cout << data[packet_it][byte_it]; 
            }
            std::cout << '\n';
        }

        for(int i=0; i<ninput; i++){
            /* Just send down the out stream what comes in. */
            if(i == REQUESTED_BUFFER_RATIO/2)
                out[0] = in[i];
        }  

        /* Tell runtime system how many input items we consumed on input stream. */
        consume_each (ninput);

        /* Tell runtime system how many output items we produced. */
        return noutput_items;
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
