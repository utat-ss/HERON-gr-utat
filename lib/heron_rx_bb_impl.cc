/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "heron_rx_bb_impl.h"
#include "debug_logger.h"
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
    #ifdef DEBUG_LOGGER
    debug_logger << "starting debug at time = " << std::time(0) << '\n';
    debug_logger << "running constructor\n";
    debug_logger << std::hex;
    debug_logger << "running constructor, debug: " << DEBUG_FILE << '\n';
    #endif

    d_pkt.clear();
}

heron_rx_bb_impl::~heron_rx_bb_impl(){
    DEBUG_STREAM("running destructor\n");
}

void heron_rx_bb_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    DEBUG_STREAM("running forecast\n");

    float data_len = 128.f/2; // how big data field 2 is expeceted to be in bytes
    float dec_per_packet = 9.f; // dec = decoration = prefix + suffix (in bytes)
    float packet_len = data_len + dec_per_packet;
    float npackets = noutput_items / (data_len+1); // add 1 for extra carrige return

    ninput_items_required[0] = (int)(8*npackets*packet_len);
}

int heron_rx_bb_impl::general_work(int noutput_items,
                                   gr_vector_int& ninput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    DEBUG_STREAM(std::dec);
    DEBUG_STREAM("running general work with noutput_items = " << noutput_items << '\n');
    DEBUG_STREAM(std::hex);

    const int ninput_max = ninput_items[0];
    const int noutput_max = noutput_items;

    int ninput = 0; // number of bytes actually processed
    int noutput = 0; // number of bytes actually outputted

    auto fill_output_buf_if_available = [&](){
        while(d_pkt.state == finished_packet && noutput < noutput_max){
            out[noutput] = d_pkt.pop_data_if_avail();
            noutput++;
        }
    };

    fill_output_buf_if_available();

    while(ninput < ninput_max && noutput < noutput_max){
        process_byte(in[ninput]);
        ninput++;
        fill_output_buf_if_available();
    }

    consume_each(ninput);
    return noutput;

}

void
heron_rx_bb_impl::process_byte (uint8_t bit)
{

    switch (d_pkt.state) {

        case getting_preamble:

            d_pkt.append_bit_to_preamble(bit);

            DEBUG_STREAM("getting preamble: 0x" << d_pkt.preamble << '\n');

            if (d_pkt.preamble_identified()) {
                d_pkt.state = getting_sync_word;
                DEBUG_STREAM(" ::: Finished getting preamble ::: " << std::endl);
            }
            break;

        case getting_sync_word:

            d_pkt.append_bit_to_sync_word(bit);

            DEBUG_STREAM("getting sync word: 0x" << (int)d_pkt.sync_word << '\n');

            if(d_pkt.sync_word_identified()){
                d_pkt.counter = 0;
                d_pkt.state = getting_size_byte;
                DEBUG_STREAM(" ::: Finished getting sync word ::: " << std::endl);
            }else if(d_pkt.sync_word_timeout()){
                d_pkt.clear();
            }
            break;

        case getting_size_byte:
            
            d_pkt.append_bit_to_size_byte(bit);

            DEBUG_STREAM("getting size byte: 0x" << (int)d_pkt.size_byte << '\n');

            if(d_pkt.size_byte_identified()){
                d_pkt.counter = 0;
                d_pkt.state = getting_data;
                DEBUG_STREAM(std::dec);
                DEBUG_STREAM(" ::: Finished getting size byte: " << (int)d_pkt.size_byte << " bytes expected ::: " << std::endl);
                DEBUG_STREAM(std::hex);
            }
            break;

        case getting_data:

            DEBUG_STREAM("getting data\n");

            d_pkt.append_bit_to_data(bit);
            if(d_pkt.data_identified()){
                d_pkt.counter = 0;
                d_pkt.state = getting_checksum;
                DEBUG_STREAM(" ::: Finished getting data ::: " << std::endl);
            }
            break;

        case getting_checksum:
        
            d_pkt.append_bit_to_checksum(bit);

            DEBUG_STREAM("getting checksum: " << d_pkt.checksum << '\n');

            if(d_pkt.checksum_identified()){

                DEBUG_STREAM(" ::: Finished getting checksum ::: " << std::endl);

                if(d_pkt.checksum_matches()){
                    #ifdef DEBUG_LOGGER
                    debug_logger << "PACKET RECEIVED - VALID CHECKSUM\n";
                    debug_logger << "Packet length: " << std::to_string(d_pkt.size_byte) << std::endl;
                    debug_logger << "Packet contents (HEX): " << std::endl;
                    for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                        debug_logger << "0x" << std::setfill('0') << std::setw(2) << (int)d_pkt.data[j] << " ";
                    }
                    debug_logger << std::endl;
                    debug_logger << "Packet contents (regular text): " << std::endl;
                    for (uint8_t j = 0; j < d_pkt.size_byte; j++) {
                        debug_logger << d_pkt.data[j] << " ";
                    }
                    debug_logger << std::endl;
                    debug_logger << "================================" << std::endl;
                    #endif

                    d_pkt.counter = 0;
                    d_pkt.state = finished_packet;
                    d_pkt.data.push_back(0x0D); // append carriage return for ESTTC protocol

                }else{
                    DEBUG_STREAM("PACKET RECEIVED - INVALID CHECKSUM\n");
                    d_pkt.clear();
                }
            }
            break;

        default:
            DEBUG_STREAM("defaulted out\n");

            d_pkt.clear();
            d_pkt.state = getting_preamble;
            break;
    }

}

}
}
