/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_HEADER_FORMAT_ESTTC_H
#define INCLUDED_UTAT_HERON_HEADER_FORMAT_ESTTC_H

#include <gnuradio/UTAT_HERON/api.h>
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
    static sptr make();
    header_format_esttc();
    ~header_format_esttc();
    bool format(
        int nbytes_in,
        const unsigned char* input,
        pmt::pmt_t& output,
        pmt::pmt_t& info) override;
    size_t header_nbits() const override;
protected:
    void enter_have_sync() override;
    void enter_have_header(int payload_len) override;
    void enter_search() override;
    bool header_ok() override;
    int header_payload() override;
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_HEADER_FORMAT_ESTTC_H */
