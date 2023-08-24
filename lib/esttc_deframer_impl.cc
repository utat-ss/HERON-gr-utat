/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "esttc_deframer_impl.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/UTAT_HERON/header_format_esttc.h>
#include <gnuradio/digital/correlate_access_code_tag_bb.h>
#include <gnuradio/digital/header_payload_demux.h>
#include <gnuradio/digital/protocol_parser_b.h>
#include <gnuradio/digital/crc_check.h>
#include <gnuradio/pdu/tagged_stream_to_pdu.h>
#include <gnuradio/blocks/repack_bits_bb.h>
#include <gnuradio/blocks/tagged_stream_multiply_length.h>

namespace gr {
namespace UTAT_HERON {

esttc_deframer::sptr esttc_deframer::make(float samp_rate)
{
    return gnuradio::make_block_sptr<esttc_deframer_impl>(samp_rate);
}

class prepend_length : public gr::sync_block{

private:
    int skip_bytes;
    void handle_msg(const pmt::pmt_t& pdu){
        auto data = pmt::u8vector_elements(pmt::cdr(pdu));
        auto start = data.begin();
        data.insert(start, data.size()-skip_bytes);
        auto new_cdr = pmt::init_u8vector(data.size(), data);
        auto new_pdu = pmt::cons(pmt::car(pdu), new_cdr);
        message_port_pub(pmt::intern("pdu_out"), new_pdu);
    }
public:

    prepend_length(int skip_bytes=0):
        gr::sync_block(
            "prepend_length",
            gr::io_signature::make(0,0,0),
            gr::io_signature::make(0,0,0)
        ),
        skip_bytes(skip_bytes)
    {
        message_port_register_in(pmt::intern("pdu_in"));
        message_port_register_out(pmt::intern("pdu_out"));

        set_msg_handler(pmt::intern("pdu_in"), [this](const pmt::pmt_t& pdu){
            this->handle_msg(pdu);
        });
    }

    int work(
        int noutput_items,
        gr_vector_const_void_star& input_items,
        gr_vector_void_star& output_items
    ) override
    {
        return 0;
    }

    typedef std::shared_ptr<prepend_length> sptr;
    
    static sptr make(int skip_bytes=0){
        return gnuradio::make_block_sptr<prepend_length>(skip_bytes);
    }
};

class strip_prepended_length : public gr::sync_block{
private:
    void handle_msg(const pmt::pmt_t& pdu){
        auto data = pmt::u8vector_elements(pmt::cdr(pdu));
        auto start = data.begin();
        auto end = data.end();
        data = std::vector<uint8_t>(start+1, end);
        auto new_cdr = pmt::init_u8vector(data.size(), data);
        auto new_pdu = pmt::cons(pmt::car(pdu), new_cdr);
        message_port_pub(pmt::intern("pdu_out"), new_pdu);
    }
public:

    strip_prepended_length():
        gr::sync_block(
            "strip_prepended_length",
            gr::io_signature::make(0,0,0),
            gr::io_signature::make(0,0,0)
        )
    {
        message_port_register_in(pmt::intern("pdu_in"));
        message_port_register_out(pmt::intern("pdu_out"));

        set_msg_handler(pmt::intern("pdu_in"), [this](const pmt::pmt_t& pdu){
            this->handle_msg(pdu);
        });
    }

    int work(
        int noutput_items,
        gr_vector_const_void_star& input_items,
        gr_vector_void_star& output_items
    ) override
    {
        return 0;
    }

    typedef std::shared_ptr<strip_prepended_length> sptr;
    
    static sptr make(){
        return gnuradio::make_block_sptr<strip_prepended_length>();
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

    message_port_register_hier_out(pmt::intern("crc_ok"));
    message_port_register_hier_out(pmt::intern("crc_fail"));

    auto access_code = "101010101010101010101010101010101010101001111110";

    auto hdr_format = UTAT_HERON::header_format_esttc::make(access_code, 3, 1, 16);
    auto find_access_code = gr::digital::correlate_access_code_tag_bb::make(&access_code[8], 0, "time_est");
    auto hdr_payload_demux = gr::digital::header_payload_demux::make(8, 1, 0, "payload symbols", "time_est", false, sizeof(uint8_t), "rx_time", samp_rate);
    auto parser = gr::digital::protocol_parser_b::make(hdr_format);
    auto pack_bits = gr::blocks::repack_bits_bb::make(1, 8, "", false, gr::GR_MSB_FIRST);
    auto scale_length = gr::blocks::tagged_stream_multiply_length::make(sizeof(uint8_t), "payload symbols", 1./8);
    auto stream_to_pdu = gr::pdu::tagged_stream_to_pdu::make(gr::types::byte_t, "payload symbols");
    auto prepend_length = prepend_length::make(2);
    auto crc = gr::digital::crc_check::make(16, 0x1021, 0xFFFF, 0x0000, false, false, false, true, 0);
    auto strip_prepended_len = strip_prepended_length::make();

    connect(self(), 0, find_access_code, 0);
    connect(find_access_code, 0, hdr_payload_demux, 0);
    connect(hdr_payload_demux, 0, parser, 0);
    msg_connect(parser, "info", hdr_payload_demux, "header_data");
    connect(hdr_payload_demux, 1, pack_bits, 0);
    connect(pack_bits, 0, scale_length, 0);
    connect(scale_length, 0, stream_to_pdu, 0);
    msg_connect(stream_to_pdu, "pdus", prepend_length, "pdu_in");
    msg_connect(prepend_length, "pdu_out", crc, "in");
    msg_connect(crc, "ok", strip_prepended_len, "pdu_in");
    msg_connect(crc, "fail", self(), "crc_fail");
    msg_connect(strip_prepended_len, "pdu_out", self(), "crc_ok");

}

/*
 * Our virtual destructor.
 */
esttc_deframer_impl::~esttc_deframer_impl() {}


} /* namespace UTAT_HERON */
} /* namespace gr */
