/* -*- c++ -*- */
/*
 * Copyright 2021 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "heron_rx_bb_impl.h" // iostream included here
#include <fstream> // https://www.cplusplus.com/doc/tutorial/files/




namespace gr {
  namespace utat3_9 {

    // =============== debug =================
    std::string debug_path = "/home/yongdali/Documents/GitHub/UTAT_Space/ground-station/recordings/";

    // =============================

    using input_type = uint8_t;
    using output_type = uint8_t;
    heron_rx_bb::sptr
    heron_rx_bb::make()
    {
      return gnuradio::make_block_sptr<heron_rx_bb_impl>(
        );
    }


    /*
     * The private constructor
     */
    heron_rx_bb_impl::heron_rx_bb_impl()
      : gr::block("heron_rx_bb",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint8_t)))
    {
        // =======================================================
        // debugging
        // remove previous file
        // just open a new file in regular open mode, and it'll overwrite previous content
        std::ofstream myfile;
        myfile.open (debug_path + "debug.txt");
        myfile << "starting debug at time = " << std::time(0) << "\n";
        myfile << "running constructor\n";

        std::cout << "running constructor\n";

        myfile.close();
        // =======================================================

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
        // =======================================================
        // debugging
        std::cout<< "running forecast \n";

        std::ofstream myfile;
        myfile.open (debug_path + "debug.txt", std::ios::out | std::ios::app);
        myfile << "running forecast\n";
        // =======================================================

          ninput_items_required[0] = noutput_items;
          /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    heron_rx_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        // =======================================================
        // debugging
        std::cout<< "running general work\n";


        std::ofstream myfile;
        myfile.open (debug_path + "debug.txt", std::ios::out | std::ios::app);

        // set basefield format to decimal, since noutput_items is a decimal 
        std::dec;
        myfile << "running general work with noutput_items = " << noutput_items << "\n";
        // =======================================================


        const uint8_t *in = (const uint8_t *) input_items[0];
        uint8_t *out = (uint8_t *) output_items[0];

        int num_bytes_processed = 0;
        int output_size = 0;

        for (int i = 0; i < noutput_items; i++) {
            // Assume will be consuming noutput_items (full size), and leave if we find a packet
            process_byte ( in[i] );
            if (d_state == finished_packet) {
                if (crc16(d_pkt.data, d_pkt.size_byte) == d_pkt.checksum) {
                    // 1a) IF CHECKSUM VALID, set up output (out[] and output_size)

                    myfile << "PACKET RECEIVED - VALID CHECKSUM" << std::endl;
                    output_size = (int)d_pkt.size_byte + 1;
                    for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                        out[j] = d_pkt.data[j];
                    }
                    out[d_pkt.size_byte] = 0x0D; // concatenate with a Carriage Return
                } else {
                    // 1b) IF CHECKSUM INVALID, print error message, flush out[]
                    myfile << "PACKET FAILED - INVALID CHECKSUM" << std::endl;
                    output_size = 1;
                    out[0] = 0x0D; // send a carriage return
                }
                // 2) Print packet content as HEX and TEXT 
                myfile << "Packet length: " << std::to_string(d_pkt.size_byte) << std::endl;
                myfile << "Packet contents (HEX): " << std::endl;
                for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                    myfile << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)d_pkt.data[j] << " ";
                }
                myfile << std::endl;
                myfile << std::dec;
                myfile << "Packet contents (regular text): " << std::endl;
                for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                    myfile << "0x" << std::to_string(d_pkt.data[j]) << " ";
                }
                myfile << std::endl;
                myfile << "================================" << std::endl;
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

            myfile << "did not get a packet in general_work\n";
            // return noutput_items;
            return 0;
        }

        myfile.close();
    }

    void
    heron_rx_bb_impl::process_byte (uint8_t bit_byte)
    {

        std::ofstream myfile;
        myfile.open (debug_path + "debug.txt", std::ios::out | std::ios::app);

        // set baesfield format to hex, for d_pkt.preamble
        std::hex;
        char buf[1000];
        snprintf(buf, 1000, "entering process_byte with d_pkt.preamble = %#X\n", d_pkt.preamble);
        myfile << buf;

        // This block goes before a Pack K(8) Bits block because we need flexible alignment - it will pack them on the output
        switch (d_state) {
            case getting_preamble:
                myfile << "getting preamble\n";
                // Update stored preamble
                if (bit_byte == 0x01) {
                    d_pkt.preamble = (d_pkt.preamble << 1) + 1;
                } else if (bit_byte == 0x00) {
                    d_pkt.preamble = d_pkt.preamble << 1;
                } else {
                  myfile << " Don't use Pack K Bits before this block, must be 0x00 or 0x01 input." << std::endl;
                }            
                // Once you get 31b or 32b of preamble, update state and get the sync word.
                if (d_pkt.preamble == 0x555555) {
                    d_state = getting_sync_word;
                    myfile << " ::: Finished getting preamble ::: " << std::endl;
                }
                break;

            case getting_sync_word:
                myfile << "getting sync word\n";

                // No way to know if preamble ended aligned w previous step - use d_counter to quit if you don't find 0x7E soon
                if (d_counter < 50) { // 32 or 33 should be sufficient, but 50 allows for some coincidental 010101 pattern before the real preamble
                    if (bit_byte == 0x01) {
                        d_pkt.sync_word = (d_pkt.sync_word << 1) + 1;
                    } else if (bit_byte == 0x00) {
                        d_pkt.sync_word = d_pkt.sync_word << 1;
                    }
                    d_counter++;
                    // Once you get the right sync word, update the state and move to get the byte size.
                    if (d_pkt.sync_word == 0x7E) {
                        d_counter = 0;
                        d_state = getting_size_byte;
                        myfile << " ::: Finished getting sync word ::: " << std::endl;
                    }
                } else {
                    clear_packet(); // this resets the state and sets counter to 0
                }
                break;

            case getting_size_byte:
                myfile << "getting size byte\n";
                // Now have fixed-size fields, so use counter for that
                if (d_counter < 8) {
                    if (bit_byte == 0x01) {
                        d_pkt.size_byte = (d_pkt.size_byte << 1) + 1;
                    } else if (bit_byte == 0x00) {
                        d_pkt.size_byte = d_pkt.size_byte << 1;
                    }
                    d_counter++;
                    // Once you've read in the fixed byte size field, print the size out and update the state.
                    if (d_counter == 8) {
                        d_counter = 0;
                        d_state = getting_data;
                        myfile << " ::: Finished getting size byte: " << std::to_string(d_pkt.size_byte) << " bytes expected ::: " << std::endl;
                    }
                }
                break;

            case getting_data:
                myfile << "getting data\n";

                // Use d_pkt.size_byte to get fixed size of the data field
                //   e.g. if d_pkt.size_byte == 0x0A, should be reading 10 bytes == 80 "bit bytes"
                if (d_counter < 8*d_pkt.size_byte) { // -- will always end on counter % 8 == 7, which is good
                    // If counter is a multiple of 8 you add a new byte
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
                    // Once the data has been read out, print that the process is done, then move to checksum. 
                    if (d_counter == 8*d_pkt.size_byte) {
                        d_counter = 0;
                        d_state = getting_checksum;
                        myfile << " ::: Finished getting data ::: " << std::endl;
                    }
                }
                break;

            case getting_checksum:
                myfile << "getting checksum\n";
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
                        myfile << " ::: Finished getting checksum ::: " << std::endl;
                    }
                }
                break;


            default:
                myfile << "defaulted out\n";

                clear_packet();
                d_state = getting_preamble;
                break;
        }

        myfile.close();
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


  } /* namespace utat3_9 */
} /* namespace gr */

