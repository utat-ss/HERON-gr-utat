/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_ESTTC_FRAMER_H
#define INCLUDED_UTAT_HERON_ESTTC_FRAMER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief Generates valid ESTTC packets with the input as the payload
 * \ingroup framing
 *
 */
class UTAT_HERON_API esttc_framer : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<esttc_framer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of UTAT_HERON::esttc_framer.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::esttc_framer's
     * constructor is in a private implementation
     * class. UTAT_HERON::esttc_framer::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_ESTTC_FRAMER_H */
