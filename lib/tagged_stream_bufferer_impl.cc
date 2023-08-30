/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tagged_stream_bufferer_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace UTAT_HERON {

using input_type = gr_complex;
using output_type = gr_complex;

tagged_stream_bufferer::sptr tagged_stream_bufferer::make(
    std::size_t buffer_size,
    const std::string& packet_len_tag,
    const std::string& trailer_tag)
{
    return gnuradio::make_block_sptr<tagged_stream_bufferer_impl>(buffer_size, packet_len_tag, trailer_tag);
}


/*
 * The private constructor
 */
tagged_stream_bufferer_impl::tagged_stream_bufferer_impl(std::size_t buffer_size, const std::string& packet_len_tag, const std::string& trailer_tag)
    : gr::block("tagged_stream_bufferer",
        gr::io_signature::make(1,1,sizeof(input_type)),
        gr::io_signature::make(1,1,sizeof(output_type))),
    d_state(STATE_BLOCK),
    d_packet_len(0),
    d_remaining_padding(0),
    d_buffer_size(buffer_size),
    d_lengthtag(pmt::mp(packet_len_tag)),
    d_tailtag(pmt::mp(trailer_tag))
{
    set_tag_propagation_policy(TPP_DONT);
}

/*
 * Our virtual destructor.
 */
tagged_stream_bufferer_impl::~tagged_stream_bufferer_impl() {}

void tagged_stream_bufferer_impl::forecast(int noutput_items,
                                           gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items;
}

int tagged_stream_bufferer_impl::general_work(int noutput_items,
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
        d_packet_len += n;
    };

    auto can_output = [&](int n){
        return noutput_items - nproduced - n >= 0;
    };

    auto can_consume = [&](int n){
        return n_in - nconsumed - n >= 0;
    };

    auto consume_drop_to_floor = [&](int n){
        nconsumed += n;
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
        d_packet_len = 0;
        in_to_out_packet(1);
        tag_offset_to_last_output_item(tag);
        add_item_tag(0, tag);
        d_state = STATE_PACKET;
    };
    
    auto output_padding = [&](int len){
        for(int i = 0; i < len; i++)
            out[nproduced++] = output_type();
        d_remaining_padding -= len;
    };

    // auto handle_state_block = [&](int len){
    //     if(can_consume(len)){
    //         consume_drop_to_floor(len);
    //         if(remaining_output() != 0)
    //             return true;
    //     }else
    //         consume_drop_to_floor(remaining_input());
    //     return false;
    // };


    auto handle_state_block_until_packet = [&](int len){
        if(can_consume(len)){
            consume_drop_to_floor(len);
            if(remaining_output() != 0){
                d_state = STATE_PACKET_HEADER;
                return true;
            }
        }else consume_drop_to_floor(remaining_input());
        d_state = STATE_BLOCK;
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

    // auto handle_state_packet = [&](int len){
    //     if(can_move_in_to_out(len)){
    //         in_to_out_packet(len);
    //         if(remaining_output() != 0)
    //             return true;
    //     }else
    //         in_to_out_packet(remaining_move());
    //     return false;
    // };

    auto handle_state_packet_until_trailer = [&](int len){
        if(can_move_in_to_out(len)){
            in_to_out_packet(len);
            if(remaining_output() != 0){
                d_state = STATE_PADDING;
                d_remaining_padding = d_buffer_size - d_packet_len;
                if(d_remaining_padding <= 0)
                    d_state = STATE_BLOCK;
                return true;
            }
        }else in_to_out_packet(remaining_move());
        d_state = STATE_PACKET;
        return false;
    };

    auto handle_state_padding = [&](){
        if(can_output(d_remaining_padding)){
            output_padding(d_remaining_padding);
            d_state = STATE_BLOCK;
            if(remaining_output() != 0)
                return true;
        }else output_padding(remaining_output());
        return false;
    };

    bool keep_going;

    switch(d_state){
        case STATE_BLOCK:
        case STATE_PACKET:
            break;
        case STATE_PADDING:
            keep_going = handle_state_padding();
            if(!keep_going) return finish();
            break;
        case STATE_BLOCK_UNTIL_PACKET:
        case STATE_PACKET_HEADER:
        case STATE_PACKET_UNTIL_TRAILER:
            throw std::runtime_error("something went terribly wrong");        
    }

    std::vector<tag_t> tags;
    get_tags_in_range(tags, 0, nitems_read(0)+nconsumed, nitems_read(0)+n_in);

    for (size_t i = 0; i < tags.size(); i++){
        auto tag = tags[i];
        int len = tag.offset - nitems_read(0) - nconsumed;

        if (pmt::eqv(tag.key, d_lengthtag) && d_state == STATE_BLOCK){

            d_state = STATE_BLOCK_UNTIL_PACKET;
            
            keep_going = handle_state_block_until_packet(len);
            if(!keep_going) return finish();

            keep_going = handle_state_packet_header(tag);
            if(!keep_going) return finish();

        }else if(pmt::eqv(tag.key, d_tailtag) && d_state == STATE_PACKET){

            d_state = STATE_PACKET_UNTIL_TRAILER;

            keep_going = handle_state_packet_until_trailer(len);
            if(!keep_going) return finish();

            keep_going = handle_state_padding();
            if(!keep_going) return finish();

        }
    }

    switch(d_state){
        case STATE_BLOCK:
            consume_drop_to_floor(remaining_input());
            break;
        case STATE_PACKET:
            in_to_out_packet(remaining_move());
            break;
        case STATE_BLOCK_UNTIL_PACKET:
        case STATE_PACKET_HEADER:
        case STATE_PACKET_UNTIL_TRAILER:
        case STATE_PADDING:
            throw std::runtime_error("something went terribly wrong");
    }

    return finish();
}

} /* namespace UTAT_HERON */
} /* namespace gr */
