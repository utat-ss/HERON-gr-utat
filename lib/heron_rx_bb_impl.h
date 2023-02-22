/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_HERON_RX_BB_IMPL_H
#define INCLUDED_UTAT_HERON_HERON_RX_BB_IMPL_H

#include "heron_packet.h"
#include <gnuradio/UTAT_HERON/heron_rx_bb.h>
#include <string>
#include <cstdint>
#include <deque>
#include <fstream>

#ifdef DEBUG_FILE
#define DEBUG_STREAM(x) debug_file << x
#else
#define DEBUG_STREAM(x)
#endif

namespace gr {
namespace UTAT_HERON {

class heron_rx_bb_impl : public heron_rx_bb
{
private:
    #ifdef DEBUG_FILE
    std::ofstream debug_file;
    #endif
    heron_packet d_pkt;
public:
    heron_rx_bb_impl();
    ~heron_rx_bb_impl();
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);
    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
    void process_byte(uint8_t bit_byte);
    uint16_t crc16(const std::deque<uint8_t> &data, uint8_t data_length);
};

}
}

#endif
