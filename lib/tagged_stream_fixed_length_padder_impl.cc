/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tagged_stream_fixed_length_padder_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace UTAT_HERON {

using input_type = uint8_t;
using output_type = uint8_t;

tagged_stream_fixed_length_padder::sptr tagged_stream_fixed_length_padder::make(
    const std::string& len_tag_key,
    float final_samples_per_symbol,
    int final_buffer_len
    // float threshold
)
{
    return gnuradio::make_block_sptr<tagged_stream_fixed_length_padder_impl>(
        len_tag_key,
        final_samples_per_symbol,
        final_buffer_len
        // threshold
    );
}


/*
 * The private constructor
 */
tagged_stream_fixed_length_padder_impl::tagged_stream_fixed_length_padder_impl(
    const std::string& len_tag_key,
    float final_samples_per_symbol,
    int final_buffer_len
    // float threshold
) :
    gr::tagged_stream_block(
        "tagged_stream_fixed_length_padder",
        gr::io_signature::make(1,1,sizeof(input_type)),
        gr::io_signature::make(1,1,sizeof(output_type)),
        len_tag_key),
    d_sps(final_samples_per_symbol),
    d_buffer_len(final_buffer_len),
    // d_threshold(threshold),
    // d_min_samps(std::ceil(d_buffer_len + d_threshold)),
    d_samps_out(0)
{}

/*
 * Our virtual destructor.
 */
tagged_stream_fixed_length_padder_impl::~tagged_stream_fixed_length_padder_impl() {}

int tagged_stream_fixed_length_padder_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    // int len = static_cast<int>(std::ceil((d_min_samps - d_samps_out)/d_sps));
    int len = d_buffer_len + 1*d_sps;
    if(len/d_sps < ninput_items[0])
        throw std::runtime_error("Input needs to be smaller!");
    return len;
}

int tagged_stream_fixed_length_padder_impl::work(int noutput_items,
                                                 gr_vector_int& ninput_items,
                                                 gr_vector_const_void_star& input_items,
                                                 gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    int produced = 0;

    std::memcpy(out, in, ninput_items[0]*sizeof(input_type));
    d_samps_out += ninput_items[0]*d_sps;
    produced = ninput_items[0];

    while(d_samps_out < d_buffer_len && produced < noutput_items){
        out[produced++] = 0xCC;
        d_samps_out += d_sps;
    }

    if(d_samps_out < d_buffer_len)
        throw std::runtime_error("Output needs to be larger!");
    
    d_samps_out -= produced*d_sps;
    return produced;

}

} /* namespace UTAT_HERON */
} /* namespace gr */
