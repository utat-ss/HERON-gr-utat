/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_IMPL_H
#define INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_IMPL_H

#include <gnuradio/UTAT_HERON/tagged_stream_fixed_length_padder.h>

namespace gr {
namespace UTAT_HERON {

class tagged_stream_fixed_length_padder_impl : public tagged_stream_fixed_length_padder
{
private:
    // Nothing to declare in this block.
    double d_sps;
    int d_buffer_len;
    uint8_t d_filler;
    int d_samps_overflow;
    int d_len;
    int d_additional_samp_overflow;
    int d_additional_symb_overflow;

protected:
    int calculate_output_stream_length(const gr_vector_int& ninput_items);

public:
    tagged_stream_fixed_length_padder_impl(
        const std::string& len_tag_key,
        double final_samples_per_symbol,
        int final_buffer_len,
        uint8_t filler,
        int additional_symb_overflow
    );
    ~tagged_stream_fixed_length_padder_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_int& ninput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_FIXED_LENGTH_PADDER_IMPL_H */
