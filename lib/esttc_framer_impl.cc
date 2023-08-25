/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "esttc_framer_impl.h"
#include "utils/common.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/UTAT_HERON/header_format_esttc.h>
#include <gnuradio/digital/protocol_formatter_async.h>
#include <gnuradio/digital/crc_append.h>
#include <gnuradio/pdu/pdu_to_tagged_stream.h>
#include <gnuradio/pdu/tagged_stream_to_pdu.h>
#include <gnuradio/blocks/tagged_stream_mux.h>
#include <pmt/pmt.h>

namespace gr {
namespace UTAT_HERON {

esttc_framer::sptr esttc_framer::make()
{
    return gnuradio::make_block_sptr<esttc_framer_impl>();
}


/*
 * The private constructor
 */
esttc_framer_impl::esttc_framer_impl()
    : gr::hier_block2("esttc_framer",
                      gr::io_signature::make(0,0,0),
                      gr::io_signature::make(0,0,0))
{
    message_port_register_hier_in(pmt::mp("pdu_in"));
    message_port_register_hier_out(pmt::mp("pdu_out"));

    auto hdr_format = UTAT_HERON::header_format_esttc::make(utils::access_code, 3, 1, utils::crc::num_bits);
    auto formatter = gr::digital::protocol_formatter_async::make(hdr_format);
    auto header_stream = gr::pdu::pdu_to_tagged_stream::make(gr::types::byte_t, "packet_len");
    auto payload_stream = gr::pdu::pdu_to_tagged_stream::make(gr::types::byte_t, "packet_len");
    auto combine = gr::blocks::tagged_stream_mux::make(1, "packet_len");
    auto stream_to_pdu = gr::pdu::tagged_stream_to_pdu::make(gr::types::byte_t, "packet_len");
    auto crc = gr::digital::crc_append::make(
        utils::crc::num_bits,
        utils::crc::poly,
        utils::crc::inital_value,
        utils::crc::final_xor,
        utils::crc::input_reflected,
        utils::crc::result_reflected,
        false, 6);

    msg_connect(self(), "pdu_in", formatter, "in");
    msg_connect(formatter, "header", header_stream, "pdus");
    msg_connect(formatter, "payload", payload_stream, "pdus");
    connect(header_stream, 0, combine, 0);
    connect(payload_stream, 0, combine, 1);
    connect(combine, 0, stream_to_pdu, 0);
    msg_connect(stream_to_pdu, "pdus", crc, "in");
    msg_connect(crc, "out", self(), "pdu_out");

}

/*
 * Our virtual destructor.
 */
esttc_framer_impl::~esttc_framer_impl() {}


} /* namespace UTAT_HERON */
} /* namespace gr */
