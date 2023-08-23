/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_ESTTC_FRAMER_IMPL_H
#define INCLUDED_UTAT_HERON_ESTTC_FRAMER_IMPL_H

#include <gnuradio/UTAT_HERON/esttc_framer.h>

namespace gr {
namespace UTAT_HERON {

class esttc_framer_impl : public esttc_framer
{
private:
    // Nothing to declare in this block.

public:
    esttc_framer_impl();
    ~esttc_framer_impl();

    // Where all the action really happens
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_ESTTC_FRAMER_IMPL_H */
