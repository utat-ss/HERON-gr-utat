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


#ifndef INCLUDED_UTAT_ENDUROSAT_FRAME_SYNC_BB_IMPL_H
#define INCLUDED_UTAT_ENDUROSAT_FRAME_SYNC_BB_IMPL_H

#include <utat/endurosat_frame_sync_bb.h>
#include <iostream>
#include <vector>

#define   MAX_DATA_LENGTH   256
#define   REQUESTED_BUFFER_RATIO 3000

namespace gr {
  namespace utat {

    class endurosat_frame_sync_bb_impl : public endurosat_frame_sync_bb
    {
     private:
      // Nothing to declare in this block.

     public:
      endurosat_frame_sync_bb_impl();
      ~endurosat_frame_sync_bb_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      // void process_packet(const uint8_t *in, const int ninput, int &stream_it, std::vector<std::vector<uint8_t> > &data);
      
      uint16_t crc16(const std::vector<uint8_t> &data_field, uint8_t data_length);
    };

  } // namespace utat
} // namespace gr

#endif /* INCLUDED_UTAT_ENDUROSAT_FRAME_SYNC_BB_IMPL_H */

