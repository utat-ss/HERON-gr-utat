/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_ESTTC_DEFRAMER_H
#define INCLUDED_UTAT_HERON_ESTTC_DEFRAMER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description of block+>
 * \ingroup UTAT_HERON
 *
 */
class UTAT_HERON_API esttc_deframer : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<esttc_deframer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of UTAT_HERON::esttc_deframer.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::esttc_deframer's
     * constructor is in a private implementation
     * class. UTAT_HERON::esttc_deframer::make is the public interface for
     * creating new instances.
     */
    static sptr make(float samp_rate);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_ESTTC_DEFRAMER_H */
