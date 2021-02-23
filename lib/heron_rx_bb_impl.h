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

#ifndef INCLUDED_UTAT_HERON_RX_BB_IMPL_H
#define INCLUDED_UTAT_HERON_RX_BB_IMPL_H

#include <utat/heron_rx_bb.h>
#include <utat/packet.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector> 

namespace gr {
  namespace utat {
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
        rx_recog_state d_state;
        heron_packet d_pkt;
        int d_counter;

     public:
        heron_rx_bb_impl();
        ~heron_rx_bb_impl();

        // Where all the action really happens
        void forecast (int noutput_items, gr_vector_int &ninput_items_required);

        int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

        void process_byte(uint8_t bit_byte);

        void clear_packet();

        uint16_t crc16(const std::vector<uint8_t> &data, uint8_t data_length);

    };

  } // namespace utat
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_RX_BB_IMPL_H */
