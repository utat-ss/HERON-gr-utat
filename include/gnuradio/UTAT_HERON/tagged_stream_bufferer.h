/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_TAGGED_STREAM_BUFFERER_H
#define INCLUDED_UTAT_HERON_TAGGED_STREAM_BUFFERER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description of block+>
 * \ingroup UTAT_HERON
 *
 */
class UTAT_HERON_API tagged_stream_bufferer : virtual public gr::block
{
public:
    typedef std::shared_ptr<tagged_stream_bufferer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of UTAT_HERON::tagged_stream_bufferer.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::tagged_stream_bufferer's
     * constructor is in a private implementation
     * class. UTAT_HERON::tagged_stream_bufferer::make is the public interface for
     * creating new instances.
     */
    static sptr make(
        std::size_t buffer_size,
        const std::string& packet_len_tag = "packet_len",
        const std::string& trailer_tag = "trailer");
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_BUFFERER_H */
