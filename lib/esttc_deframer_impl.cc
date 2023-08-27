/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "esttc_deframer_impl.h"
#include "utils/pdu_lambda.h"
#include "utils/common.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/UTAT_HERON/header_format_esttc.h>
#include <gnuradio/digital/correlate_access_code_tag_bb.h>
#include <gnuradio/digital/header_payload_demux.h>
#include <gnuradio/digital/protocol_parser_b.h>
#include <gnuradio/digital/crc_check.h>
#include <gnuradio/pdu/tagged_stream_to_pdu.h>
#include <gnuradio/blocks/repack_bits_bb.h>
#include <gnuradio/blocks/tagged_stream_multiply_length.h>

#include <gnuradio/blocks/tag_debug.h>
#include <gnuradio/blocks/message_debug.h>
#include <fmt/ranges.h>

namespace gr {
namespace UTAT_HERON {

esttc_deframer::sptr esttc_deframer::make(float samp_rate)
{
    return gnuradio::make_block_sptr<esttc_deframer_impl>(samp_rate);
}

pmt::pmt_t prepend_len_minus_crc_bytes(pmt::pmt_t pdu){
    std::vector data(pmt::u8vector_elements(pmt::cdr(pdu)));
    uint8_t crc_nbytes = utils::crc::num_bits/8;
    data.insert(data.begin(), data.size()-crc_nbytes);
    auto new_cdr = pmt::init_u8vector(data.size(), data);
    pmt::pmt_t new_pdu = pmt::cons(pmt::car(pdu), new_cdr);
    return new_pdu;
}

pmt::pmt_t strip_prepended_len(pmt::pmt_t pdu){
    std::vector data(pmt::u8vector_elements(pmt::cdr(pdu)));
    auto start = data.begin();
    auto end = data.end();
    data = std::vector<uint8_t>(start+1, end);
    auto new_cdr = pmt::init_u8vector(data.size(), data.data());
    pmt::pmt_t new_pdu = pmt::cons(pmt::car(pdu), new_cdr);
    return new_pdu;
}


class print_stuff : public gr::block{
public:
    print_stuff():
        gr::block(
            "print_stuff",
            gr::io_signature::make(1,1,sizeof(uint8_t)),
            gr::io_signature::make(0,0,0)
        ){}

    int general_work(
        int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items) override
    {
        const uint8_t* inputs = (const uint8_t*)(input_items[0]);
        int ninputs = (int)ninput_items[0];
        const std::vector<uint8_t> bytes(inputs, inputs+ninputs);
        d_logger->info("Vectorrrr: {}\n", bytes);
        consume_each(ninputs);
        return 0;
    }

    void forecast(
        int noutput_items,
        gr_vector_int &ninput_items_required
    )override{}

    typedef std::shared_ptr<print_stuff> sptr;
    static sptr make(){
        return gnuradio::make_block_sptr<print_stuff>();
    }
};

/*
 * The private constructor
 */
esttc_deframer_impl::esttc_deframer_impl(float samp_rate)
    : gr::hier_block2("esttc_deframer",
                      gr::io_signature::make(1, 1, sizeof(uint8_t)),
                      gr::io_signature::make(0, 0, 0))
{

    message_port_register_hier_out(pmt::mp("crc_ok"));
    message_port_register_hier_out(pmt::mp("crc_fail"));

    auto hdr_format = UTAT_HERON::header_format_esttc::make(utils::access_code, 3, 1, utils::crc::num_bits);
    auto find_access_code = gr::digital::correlate_access_code_tag_bb::make(utils::trimmed_access_code, 0, "time_est");
    auto hdr_payload_demux = gr::digital::header_payload_demux::make(
        hdr_format->header_nbits_without_access_code(),
        1, 0, "payload symbols", "time_est", false, sizeof(uint8_t), "", samp_rate);
    auto parser = gr::digital::protocol_parser_b::make(hdr_format);
    auto pack_bits = gr::blocks::repack_bits_bb::make(1, 8, "", false, gr::GR_MSB_FIRST);
    auto scale_length = gr::blocks::tagged_stream_multiply_length::make(sizeof(uint8_t), "payload symbols", 1./8);
    auto stream_to_pdu = gr::pdu::tagged_stream_to_pdu::make(gr::types::byte_t, "payload symbols");
    auto prepend_length = utils::pdu_lambda::make(prepend_len_minus_crc_bytes);
    auto crc_check_blk = gr::digital::crc_check::make(
        utils::crc::num_bits,
        utils::crc::poly,
        utils::crc::inital_value,
        utils::crc::final_xor,
        utils::crc::input_reflected,
        utils::crc::result_reflected,
        false, true, 0);
    auto strip_prepended_length = utils::pdu_lambda::make(strip_prepended_len);


    auto tag_debug = gr::blocks::tag_debug::make(sizeof(uint8_t), "Name");
    auto msg_debug = gr::blocks::message_debug::make();
    auto ps = print_stuff::make();


    // connect(find_access_code, 0, tag_debug, 0);
    // msg_connect(parser, "info", msg_debug, "print");
    // connect(scale_length, 0, ps, 0);
    // msg_connect(stream_to_pdu, "pdus", msg_debug, "log");
    // msg_connect(prepend_length, "pdu_out", msg_debug, "log");
    // msg_connect(crc_check_blk, "fail", msg_debug, "log");
    // msg_connect(crc_check_blk, "ok", msg_debug, "log");
    // msg_connect(strip_prepended_length, "pdu_out", msg_debug, "log");
    // msg_connect(crc_check_blk, "fail", msg_debug, "log");


    msg_connect(strip_prepended_length, "pdu_out", self(), "crc_ok");
    msg_connect(crc_check_blk, "ok", strip_prepended_length, "pdu_in");
    msg_connect(crc_check_blk, "fail", self(), "crc_fail");
    msg_connect(prepend_length, "pdu_out", crc_check_blk, "in");

    msg_connect(stream_to_pdu, "pdus", prepend_length, "pdu_in");
    connect(scale_length, 0, stream_to_pdu, 0);
    connect(pack_bits, 0, scale_length, 0);
    connect(hdr_payload_demux, 1, pack_bits, 0);
    msg_connect(parser, "info", hdr_payload_demux, "header_data");
    connect(hdr_payload_demux, 0, parser, 0);
    connect(find_access_code, 0, hdr_payload_demux, 0);
    connect(self(), 0, find_access_code, 0);



}

/*
 * Our virtual destructor.
 */
esttc_deframer_impl::~esttc_deframer_impl() {}


} /* namespace UTAT_HERON */
} /* namespace gr */
