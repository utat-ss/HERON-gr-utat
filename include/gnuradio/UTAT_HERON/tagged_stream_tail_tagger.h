/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_TAGGED_STREAM_TAIL_TAGGER_H
#define INCLUDED_UTAT_HERON_TAGGED_STREAM_TAIL_TAGGER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description of block+>
 * \ingroup UTAT_HERON
 *
 */
class UTAT_HERON_API tagged_stream_tail_tagger : virtual public gr::block
{
public:
    typedef std::shared_ptr<tagged_stream_tail_tagger> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * UTAT_HERON::tagged_stream_tail_tagger.
     *
     * To avoid accidental use of raw pointers, UTAT_HERON::tagged_stream_tail_tagger's
     * constructor is in a private implementation
     * class. UTAT_HERON::tagged_stream_tail_tagger::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& length_tag_key, const std::string& tail_tag_key);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_TAIL_TAGGER_H */
