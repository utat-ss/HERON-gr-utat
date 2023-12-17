/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_VARIABLE_FILTER_H
#define INCLUDED_UTAT_HERON_VARIABLE_FILTER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description of block+>
 * \ingroup UTAT_HERON
 *
 */
class UTAT_HERON_API variable_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<variable_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of UTAT_HERON::variable_filter.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::variable_filter's
     * constructor is in a private implementation
     * class. UTAT_HERON::variable_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& variable_name);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_VARIABLE_FILTER_H */
