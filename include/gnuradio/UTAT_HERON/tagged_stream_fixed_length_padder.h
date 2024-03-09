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
 * \brief Generate data padding to ensure correct number of samples are generated at the end
 * \ingroup UTAT_HERON
 *
 * Adding padding to IQ data is not a good idea since IQ signals need to follow specific patterns.
 * Padding should be added to the actual data before modulation. However, we cannot determine the
 * exact number of samples generated, so this block generated enough padding to overflow, and then
 * keeps track of the overflow for the next round of padding.
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
        uint8_t filler,
        int additional_symb_overflow
    );
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_H */
