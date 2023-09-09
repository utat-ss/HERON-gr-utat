/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_H
#define INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description of block+>
 * \ingroup UTAT_HERON
 *
 */
class UTAT_HERON_API tagged_stream_fixed_length_padder
    : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<tagged_stream_fixed_length_padder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * UTAT_HERON::tagged_stream_fixed_length_padder.
     *
     * To avoid accidental use of raw pointers,
     * UTAT_HERON::tagged_stream_fixed_length_padder's constructor is in a private
     * implementation class. UTAT_HERON::tagged_stream_fixed_length_padder::make is the
     * public interface for creating new instances.
     */
    static sptr make(
        const std::string& len_tag_key,
        double final_samples_per_symbol,
        int final_buffer_len,
        uint8_t filler
    );
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_H */
