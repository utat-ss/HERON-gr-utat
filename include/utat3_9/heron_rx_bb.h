/* -*- c++ -*- */
/*
 * Copyright 2021 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT3_9_HERON_RX_BB_H
#define INCLUDED_UTAT3_9_HERON_RX_BB_H

#include <utat3_9/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace utat3_9 {

    /*!
     * \brief <+description of block+>
     * \ingroup utat3_9
     *
     */
    class UTAT3_9_API heron_rx_bb : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<heron_rx_bb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of utat3_9::heron_rx_bb.
       *
       * To avoid accidental use of raw pointers, utat3_9::heron_rx_bb's
       * constructor is in a private implementation
       * class. utat3_9::heron_rx_bb::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace utat3_9
} // namespace gr

#endif /* INCLUDED_UTAT3_9_HERON_RX_BB_H */

