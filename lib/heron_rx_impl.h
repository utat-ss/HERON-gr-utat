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

#ifndef INCLUDED_UTAT_HERON_RX_IMPL_H
#define INCLUDED_UTAT_HERON_RX_IMPL_H

#include <utat/heron_rx.h>

namespace gr {
  namespace utat {

    class heron_rx_impl : public heron_rx
    {
     private:
      /* shift registers to hold the different components of an endurosat packet */
      uint32_t packet_training_field;
      uint8_t packet_data_flag;
      uint8_t packet_data_length;
      std::vector<uint8_t> packet_data_field;
      uint16_t packet_crc_checksum;

      /* variable which holds the process state */
      process_state state;

      /* counter for finding the data flag */
      int counter;

     public:
      heron_rx_impl(char const * filename);
      ~heron_rx_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      std::vector<uint8_t> process_byte(uint8_t byte);

      void clear_all_packet_reg();

      uint16_t crc16(const std::vector<uint8_t> &data_field, uint8_t data_length);
    };

  } // namespace utat
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_RX_IMPL_H */

