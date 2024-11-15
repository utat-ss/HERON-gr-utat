/* -*- c++ -*- */
/*
 * Copyright 2024 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_ZERO_PADDER_H
#define INCLUDED_UTAT_HERON_ZERO_PADDER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description of block+>
 * \ingroup UTAT_HERON
 *
 */
class UTAT_HERON_API zero_padder : virtual public gr::block
{
public:
    typedef std::shared_ptr<zero_padder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of UTAT_HERON::zero_padder.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::zero_padder's
     * constructor is in a private implementation
     * class. UTAT_HERON::zero_padder::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_ZERO_PADDER_H */
