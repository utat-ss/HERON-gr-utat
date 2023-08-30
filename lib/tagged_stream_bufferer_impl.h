/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_TAGGED_STREAM_BUFFERER_IMPL_H
#define INCLUDED_UTAT_HERON_TAGGED_STREAM_BUFFERER_IMPL_H

#include <gnuradio/UTAT_HERON/tagged_stream_bufferer.h>

namespace gr {
namespace UTAT_HERON {

class tagged_stream_bufferer_impl : public tagged_stream_bufferer
{
private:
    enum state_t{
        STATE_BLOCK,
        STATE_BLOCK_UNTIL_PACKET,
        STATE_PACKET_HEADER,
        STATE_PACKET,
        STATE_PACKET_UNTIL_TRAILER,
        STATE_PADDING
    };
    state_t d_state;
    long d_packet_len;
    long d_remaining_padding;
    std::size_t d_buffer_size;
    const pmt::pmt_t d_lengthtag;
    const pmt::pmt_t d_tailtag;

public:
    tagged_stream_bufferer_impl(std::size_t buffer_size, const std::string& packet_len_tag, const std::string& trailer_tag);
    ~tagged_stream_bufferer_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_BUFFERER_IMPL_H */
