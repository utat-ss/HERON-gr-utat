/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_HEADER_FORMAT_ESTTC_H
#define INCLUDED_UTAT_HERON_HEADER_FORMAT_ESTTC_H

#include <gnuradio/UTAT_HERON/api.h>
#include <gnuradio/logger.h>
#include <gnuradio/digital/header_format_default.h>

namespace gr {
namespace UTAT_HERON {

/*!
 * \brief <+description+>
 *
 */
class UTAT_HERON_API header_format_esttc : public gr::digital::header_format_default
{
public:
    typedef std::shared_ptr<header_format_esttc> sptr;
    static sptr make(const std::string& access_code, int threshold = 0, int bps = 1, int trailer_nbits = 0);
    header_format_esttc(const std::string& access_code, int threshold, int bps, int trailer_nbits);
    ~header_format_esttc();
    bool format(
        int nbytes_in,
        const unsigned char* input,
        pmt::pmt_t& output,
        pmt::pmt_t& info) override;
    bool parse(int nbits_in,
        const unsigned char* input,
        std::vector<pmt::pmt_t>& info,
        int& nbits_processed) override;
    size_t header_nbits() const override;
    size_t header_nbits_without_access_code() const;
protected:
    int d_trailer_nbits;
    void enter_have_sync() override;
    void enter_have_header(int payload_len) override;
    void enter_search() override;
    bool header_ok() override;
    int header_payload() override;
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_HEADER_FORMAT_ESTTC_H */
