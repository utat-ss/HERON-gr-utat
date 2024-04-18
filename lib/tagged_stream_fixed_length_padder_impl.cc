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
    double final_samples_per_symbol,
    int final_buffer_len,
    uint8_t filler,
    int additional_symb_overflow
)
{
    return gnuradio::make_block_sptr<tagged_stream_fixed_length_padder_impl>(
        len_tag_key,
        final_samples_per_symbol,
        final_buffer_len,
        filler,
        additional_symb_overflow
    );
}


/*
 * The private constructor
 */
tagged_stream_fixed_length_padder_impl::tagged_stream_fixed_length_padder_impl(
    const std::string& len_tag_key,
    double final_samples_per_symbol,
    int final_buffer_len,
    uint8_t filler,
    int additional_symb_overflow
) :
    gr::tagged_stream_block(
        "tagged_stream_fixed_length_padder",
        gr::io_signature::make(1,1,sizeof(input_type)),
        gr::io_signature::make(1,1,sizeof(output_type)),
        len_tag_key),
    d_sps(final_samples_per_symbol),
    d_buffer_len(final_buffer_len),
    d_filler(filler),
    d_samps_overflow(0),
    d_len(0),
    d_additional_symb_overflow(additional_symb_overflow)
{}

/*
 * Our virtual destructor.
 */
tagged_stream_fixed_length_padder_impl::~tagged_stream_fixed_length_padder_impl() {}

int tagged_stream_fixed_length_padder_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    d_len = std::ceil((d_buffer_len - d_samps_overflow)/d_sps);

    while(d_len < ninput_items[0])
        d_len += std::ceil(d_buffer_len/d_sps);
    
    d_len += d_additional_symb_overflow;
    
    d_logger->debug("len: {}", d_len);

    return d_len;
}

int tagged_stream_fixed_length_padder_impl::work(int noutput_items,
                                                 gr_vector_int& ninput_items,
                                                 gr_vector_const_void_star& input_items,
                                                 gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    int fill_len = d_len-ninput_items[0];
    auto fill_start = out+ninput_items[0];

    if(fill_len < 0){
        d_logger->alert("\n"
            "\tcalculate_output_stream_length() did not return correct length required\n"
            "\td_len = {}, ninput_items = {}", d_len, ninput_items[0]);
        return 0;
    }
    if(noutput_items < ninput_items[0]+fill_len){
        d_logger->alert("output requested is too small");
        return 0;
    }

    int samps_in_output_buffer = std::lround(d_samps_overflow + d_len*d_sps);
    d_samps_overflow = samps_in_output_buffer % d_buffer_len;

    /*
    if you want d_samps_overflow to be double, the top two lines can be converted to:
        double samps_in_output_buffer = d_samps_overflow + d_len*d_sps;
        d_samps_overflow = std::fmod(samps_in_output_buffer, (double)d_buffer_len);
    */

    std::memcpy(out, in, ninput_items[0]*sizeof(input_type));
    std::memset(fill_start, d_filler, fill_len);

    d_logger->debug("\n"
        "\tfill_len: {}\n"
        "\td_samps_overflow: {}\n",
        fill_len, d_samps_overflow);
    
    return d_len;

}

} /* namespace UTAT_HERON */
} /* namespace gr */
