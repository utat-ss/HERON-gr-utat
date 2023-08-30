/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tagged_stream_tail_tagger_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace UTAT_HERON {

using input_type = gr_complex;
using output_type = gr_complex;

tagged_stream_tail_tagger::sptr tagged_stream_tail_tagger::make(const std::string& length_tag_key, const std::string& tail_tag_key)
{
    return gnuradio::make_block_sptr<tagged_stream_tail_tagger_impl>(length_tag_key, tail_tag_key);
}

/*
 * The private constructor
 */
tagged_stream_tail_tagger_impl::tagged_stream_tail_tagger_impl(const std::string& length_tag_key, const std::string& tail_tag_key): 
    gr::block(
        "tagged_stream_tail_tagger",
        gr::io_signature::make(1, 1, sizeof(input_type)),
        gr::io_signature::make(1, 1, sizeof(output_type))),
    d_state(STATE_PASS),
    d_remaining(0),
    d_lengthtag(pmt::mp(length_tag_key)),
    d_tailtag(pmt::mp(tail_tag_key))
{
    set_tag_propagation_policy(TPP_DONT);
}

/*
 * Our virtual destructor.
 */
tagged_stream_tail_tagger_impl::~tagged_stream_tail_tagger_impl() {}

void tagged_stream_tail_tagger_impl::forecast(
    int noutput_items,
    gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items;
    if(d_state == STATE_PACKET || d_state == STATE_TRAILER)
        ninput_items_required[0]++;
}

int tagged_stream_tail_tagger_impl::general_work(
    int noutput_items,
    gr_vector_int& ninput_items,
    gr_vector_const_void_star& input_items,
    gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);
    auto n_in = static_cast<int>(ninput_items[0]);

    int nconsumed = 0;
    int nproduced = 0;

    auto in_to_out = [&](int n){
        memcpy(out+nproduced, in+nconsumed, n*sizeof(input_type));
        nconsumed += n;
        nproduced += n;
    };

    auto in_to_out_packet = [&](int n){
        in_to_out(n);
        d_remaining -= n;
    };

    auto can_output = [&](int n){
        return noutput_items - nproduced - n >= 0;
    };

    auto can_consume = [&](int n){
        return n_in - nconsumed - n >= 0;
    };

    auto can_move_in_to_out = [&](int n){
        return can_consume(n) && can_output(n);
    };

    auto remaining_output = [&](){
        return noutput_items - nproduced;
    };

    auto remaining_input = [&](){
        return n_in - nconsumed;
    };

    auto remaining_move = [&](){
        return std::min(remaining_input(), remaining_output());
    };

    auto finish = [&](){
        consume_each(nconsumed);
        return nproduced;
    };

    auto tag_offset_to_last_output_item = [&](gr::tag_t& tag){
        tag.offset = nitems_written(0) + nproduced - 1;
    };

    auto output_packet_first_symbol = [&](gr::tag_t& tag){
        d_remaining = pmt::to_long(tag.value);
        in_to_out_packet(1);
        tag_offset_to_last_output_item(tag);
        add_item_tag(0, tag);
        d_state = STATE_PACKET;
    };

    auto output_tagged_trailer = [&](){
        out[nproduced] = output_type();
        add_item_tag(0, nitems_written(0) + nproduced, d_tailtag, pmt::from_long(1));
        nproduced++;
        d_state = STATE_PASS;
    };

    auto handle_state_pass = [&](int len){
        if(can_move_in_to_out(len)){
            in_to_out(len);
            if(remaining_output() != 0)
                return true;
        }else
            in_to_out_packet(remaining_move());
        return false;
    };


    auto handle_state_pass_until_packet = [&](int len){
        if(can_move_in_to_out(len)){
            in_to_out(len);
            if(remaining_output() != 0){
                d_state = STATE_PACKET_HEADER;
                return true;
            }
        }else in_to_out_packet(remaining_move());
        d_state = STATE_PASS;
        return false;
    };

    auto handle_state_packet_header = [&](gr::tag_t& tag){
        if(can_move_in_to_out(1)){
            output_packet_first_symbol(tag);
            d_state = STATE_PACKET;
            if(remaining_output() != 0)
                return true;
        }
        return false;
    };

    auto handle_state_packet = [&](){
        if(can_move_in_to_out(d_remaining)){
            in_to_out_packet(d_remaining);
            d_state = STATE_TRAILER;
            if(remaining_output() != 0)
                return true;
        }else
            in_to_out_packet(remaining_move());
        return false;
    };

    auto handle_state_trailer = [&](){
        if(can_output(1)){
            output_tagged_trailer();
            d_state = STATE_PASS;
            if(remaining_output() != 0)
                return true;
        }
        return false;
    };

    bool keep_going;

    switch(d_state){
        case STATE_PASS:
            break;
        case STATE_PASS_UNTIL_PACKET:
        case STATE_PACKET_HEADER:
            throw std::runtime_error("something went terribly wrong");
        case STATE_PACKET:
            keep_going = handle_state_packet();
            if(!keep_going) return finish();
            // don't break
        case STATE_TRAILER:
            keep_going = handle_state_trailer();
            if(!keep_going) return finish();
    }

    std::vector<tag_t> tags;
    get_tags_in_range(tags, 0, nitems_read(0)+nconsumed, nitems_read(0)+n_in);

    for (size_t i = 0; i < tags.size(); i++){

        auto tag = tags[i];
        int len = tag.offset - nitems_read(0) - nconsumed;

        if (pmt::eqv(tag.key, d_lengthtag)){

            d_state = STATE_PASS_UNTIL_PACKET;

            keep_going = handle_state_pass_until_packet(len);
            if(!keep_going) return finish();

            keep_going = handle_state_packet_header(tag);
            if(!keep_going) return finish();

            keep_going = handle_state_packet();
            if(!keep_going) return finish();

            keep_going = handle_state_trailer();
            if(!keep_going) return finish();

        }else if(d_state == STATE_PASS){
            keep_going = handle_state_pass(len);
            if(!keep_going) return finish();

            if(can_move_in_to_out(1)){
                in_to_out(1);
                tag_offset_to_last_output_item(tag);
                add_item_tag(0, tag);
            }else return finish();

        }
    }

    if(d_state == STATE_PASS){
        in_to_out(remaining_move());
    }

    return finish();
}

} /* namespace UTAT_HERON */
} /* namespace gr */
