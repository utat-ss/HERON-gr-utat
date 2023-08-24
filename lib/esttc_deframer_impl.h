/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_ESTTC_DEFRAMER_IMPL_H
#define INCLUDED_UTAT_HERON_ESTTC_DEFRAMER_IMPL_H

#include <gnuradio/UTAT_HERON/esttc_deframer.h>

namespace gr {
namespace UTAT_HERON {

class esttc_deframer_impl : public esttc_deframer
{
private:
    // Nothing to declare in this block.

public:
    esttc_deframer_impl(float samp_rate);
    ~esttc_deframer_impl();

    // Where all the action really happens
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_ESTTC_DEFRAMER_IMPL_H */
