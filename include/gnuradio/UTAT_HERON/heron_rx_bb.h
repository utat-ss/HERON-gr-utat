/* -*- c++ -*- */
/*
 * Copyright 2023 Universitys of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_HERON_RX_BB_H
#define INCLUDED_UTAT_HERON_HERON_RX_BB_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief Only allow ESTTC packet payloads to pass
 * \ingroup UTAT_HERON
 *
 * \deprecated Replaced with UTAT_HERON::esttc_deframer
 */
class UTAT_HERON_API heron_rx_bb : virtual public gr::block
{
public:
    typedef std::shared_ptr<heron_rx_bb> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of UTAT_HERON::heron_rx_bb.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::heron_rx_bb's
     * constructor is in a private implementation
     * class. UTAT_HERON::heron_rx_bb::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

}
}

#endif
