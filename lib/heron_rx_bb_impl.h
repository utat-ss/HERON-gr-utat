/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_HERON_RX_BB_IMPL_H
#define INCLUDED_UTAT_HERON_HERON_RX_BB_IMPL_H

#include <gnuradio/UTAT_HERON/heron_rx_bb.h>
#include <gnuradio/UTAT_HERON/packet.h>
#include <string>
#include <fstream>

namespace gr {
namespace UTAT_HERON {

enum rx_recog_state {
    getting_preamble,
    getting_sync_word,
    getting_size_byte,
    getting_data,
    getting_checksum,
    finished_packet 
};

class heron_rx_bb_impl : public heron_rx_bb
{
private:
    #ifdef DEBUG_FILE
    std::ofstream debug_file;
    #endif
    
    rx_recog_state d_state;
    heron_packet d_pkt;
    int d_counter;
public:
    heron_rx_bb_impl();
    ~heron_rx_bb_impl();
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);
    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
    void process_byte(uint8_t bit_byte);
    void clear_packet();
    uint16_t crc16(const std::vector<uint8_t> &data, uint8_t data_length);
};

}
}

#endif
