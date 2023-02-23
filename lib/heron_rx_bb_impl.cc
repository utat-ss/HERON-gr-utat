/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "heron_rx_bb_impl.h"
#include "debug_logger.h"
#include <gnuradio/io_signature.h>
#include <iostream>
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
    std::cout << "running constructor, debug: " << DEBUG_FILE << '\n';
    #endif
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

    ninput_items_required[0] = static_cast<int>(8*npackets*packet_len);
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
    // NOTE: ninput is actually counting bits processed,
    // since data coming into the block should be unpacked
    // (every byte only carries a single bit)

    auto fill_output_buf_if_able = [&](){
        while(d_pkt.data_available() && noutput < noutput_max){
            out[noutput] = d_pkt.pop_data();
            noutput++;
        }
    };

    fill_output_buf_if_able();

    while(ninput < ninput_max && noutput < noutput_max){
        d_pkt.process_bit(in[ninput]);
        ninput++;
        fill_output_buf_if_able();
    }

    consume_each(ninput);
    return noutput;

}

}
}
