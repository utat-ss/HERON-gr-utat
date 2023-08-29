/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_TAGGED_STREAM_TAIL_TAGGER_IMPL_H
#define INCLUDED_UTAT_HERON_TAGGED_STREAM_TAIL_TAGGER_IMPL_H

#include <gnuradio/UTAT_HERON/tagged_stream_tail_tagger.h>

namespace gr {
namespace UTAT_HERON {

class tagged_stream_tail_tagger_impl : public tagged_stream_tail_tagger
{
private:
    enum state_t{
        STATE_PASS,
        STATE_PASS_UNTIL_PACKET,
        STATE_PACKET_HEADER,
        STATE_PACKET,
        STATE_TRAILER};
    state_t d_state;
    long d_remaining;
    const pmt::pmt_t d_lengthtag;
    const pmt::pmt_t d_tailtag;
public:
    tagged_stream_tail_tagger_impl(const std::string& length_tag_key, const std::string& tail_tag_key);
    ~tagged_stream_tail_tagger_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_TAGGED_STREAM_TAIL_TAGGER_IMPL_H */
