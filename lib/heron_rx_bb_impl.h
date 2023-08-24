/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_HERON_RX_BB_IMPL_H
#define INCLUDED_UTAT_HERON_HERON_RX_BB_IMPL_H

#include "utils/heron_packet_manager.h"
#include <gnuradio/UTAT_HERON/heron_rx_bb.h>
#include <deque>

namespace gr {
namespace UTAT_HERON {

class heron_rx_bb_impl : public heron_rx_bb
{
private:
    heron_packet_manager d_pkt;
public:
    heron_rx_bb_impl();
    ~heron_rx_bb_impl();
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);
    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

}
}

#endif
