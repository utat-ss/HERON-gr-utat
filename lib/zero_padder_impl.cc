/* -*- c++ -*- */
/*
 * Copyright 2024 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "zero_padder_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace UTAT_HERON {

using input_type = gr_complex;
using output_type = gr_complex;
zero_padder::sptr zero_padder::make()
{
    return gnuradio::make_block_sptr<zero_padder_impl>();
}


/*
 * The private constructor
 */
zero_padder_impl::zero_padder_impl()
    : gr::block("zero_padder",
                gr::io_signature::make(
                    1, 1, sizeof(input_type)),
                gr::io_signature::make(
                    1, 1, sizeof(output_type)))
{
}

/*
 * Our virtual destructor.
 */
zero_padder_impl::~zero_padder_impl() {}

void zero_padder_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = 1;
}

int zero_padder_impl::general_work(int noutput_items,
                                   gr_vector_int& ninput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    int lin = ninput_items[0];
    int lout = noutput_items;

    if(lin >= lout){
    std::memcpy(out, in, lout*sizeof(output_type));
    consume_each(lout);
    }else{
    std::memcpy(out, in, lin*sizeof(input_type));
    std::memset(out+lin, 0, lout-lin);
    consume_each(lin);
    }
    return noutput_items;
}

} /* namespace UTAT_HERON */
} /* namespace gr */
