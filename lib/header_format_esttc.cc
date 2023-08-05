/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/UTAT_HERON/header_format_esttc.h>
#include <gnuradio/io_signature.h>
#include <volk/volk_alloc.hh>

namespace gr {
namespace UTAT_HERON {

header_format_esttc::sptr header_format_esttc::make(){
    return header_format_esttc::sptr(new header_format_esttc());
}

header_format_esttc::header_format_esttc():
    header_format_default("101010101010101010101010101010101010101001111110", 3, 1)
{}

header_format_esttc::~header_format_esttc() {}

bool header_format_esttc::format(int nbytes_in,
                                   const unsigned char* input,
                                   pmt::pmt_t& output,
                                   pmt::pmt_t& info)
{

    if(nbytes_in > 128)
        throw std::runtime_error("header_format_esttc: Packet length cannot exceed 128 bits");

    // Creating the output pmt copies data; free our own here when done.
    volk::vector<uint8_t> bytes_out(header_nbytes());

    gr::digital::header_buffer header(bytes_out.data());
    header.add_field64(d_access_code, d_access_code_len);
    header.add_field8((uint8_t)(nbytes_in));

    // Package output data into a PMT vector
    output = pmt::init_u8vector(header_nbytes(), bytes_out.data());

    return true;
}


size_t header_format_esttc::header_nbits() const{
    return d_access_code_len + 8 * 1 * sizeof(uint8_t);
}

inline void header_format_esttc::enter_have_sync(){
    d_state = STATE_HAVE_SYNC;
    d_hdr_reg.clear();
}
inline void header_format_esttc::enter_have_header(int payload_len){
    d_state = STATE_SYNC_SEARCH;
    d_pkt_len = payload_len;
    d_pkt_count = 0;
}
inline void header_format_esttc::enter_search(){
    d_state = STATE_SYNC_SEARCH;
}

bool header_format_esttc::header_ok(){
    return d_hdr_reg.length() == 8 * 1 * sizeof(uint8_t);
}

int header_format_esttc::header_payload(){
    
    uint8_t len = d_hdr_reg.extract_field16(0, 8);

    d_info = pmt::make_dict();
    d_info = pmt::dict_add(
        d_info, pmt::intern("payload symbols"), pmt::from_long(8 * len / d_bps));
    return static_cast<int>(len);
}

} /* namespace UTAT_HERON */
} /* namespace gr */
