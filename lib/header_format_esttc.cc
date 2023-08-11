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

header_format_esttc::sptr header_format_esttc::make(const std::string& access_code, int threshold, int bps, int trailer_nbits){
    return header_format_esttc::sptr(new header_format_esttc(access_code, threshold, bps, trailer_nbits));
}

header_format_esttc::header_format_esttc(const std::string& access_code, int threshold, int bps, int trailer_nbits):
    header_format_default(access_code, threshold, bps),
    d_trailer_nbits(trailer_nbits)
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

bool header_format_esttc::parse(
    int nbits_in,
    const unsigned char* input,
    std::vector<pmt::pmt_t>& info,
    int& nbits_processed
){
    if(d_state == STATE_SYNC_SEARCH)
        enter_have_sync();
    
    return header_format_default::parse(nbits_in, input, info, nbits_processed);
}


size_t header_format_esttc::header_nbits() const{
    return d_access_code_len + 8 * 1 * sizeof(uint8_t);
}

inline void header_format_esttc::enter_have_sync(){
    d_state = STATE_HAVE_SYNC;
    d_hdr_reg.clear();
}
inline void header_format_esttc::enter_have_header(int payload_len){
    enter_search();
    d_pkt_len = payload_len;
    d_pkt_count = 0;
}
inline void header_format_esttc::enter_search(){
    enter_have_sync();
}

bool header_format_esttc::header_ok(){
    bool ret = d_hdr_reg.length() == 8 * 1 * sizeof(uint8_t);
    return ret;
}

int header_format_esttc::header_payload(){
    
    uint8_t len = d_hdr_reg.extract_field8(0, 8);

    d_info = pmt::make_dict();
    d_info = pmt::dict_add(
        d_info, pmt::intern("payload symbols"), pmt::from_long((8 * len + d_trailer_nbits) / d_bps));
    return static_cast<int>(len + std::ceil(d_trailer_nbits/8.0));
}

} /* namespace UTAT_HERON */
} /* namespace gr */
