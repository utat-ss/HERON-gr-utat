/* -*- c++ -*- */
/*
 * Copyright 2024 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_ZERO_PADDER_IMPL_H
#define INCLUDED_UTAT_HERON_ZERO_PADDER_IMPL_H

#include <gnuradio/UTAT_HERON/zero_padder.h>

namespace gr {
namespace UTAT_HERON {

class zero_padder_impl : public zero_padder
{
private:
    // Nothing to declare in this block.

public:
    zero_padder_impl();
    ~zero_padder_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_ZERO_PADDER_IMPL_H */
