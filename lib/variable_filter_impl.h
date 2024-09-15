/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_UTAT_HERON_VARIABLE_FILTER_IMPL_H
#define INCLUDED_UTAT_HERON_VARIABLE_FILTER_IMPL_H

#include <gnuradio/UTAT_HERON/variable_filter.h>

namespace gr {
namespace UTAT_HERON {

class variable_filter_impl : public variable_filter
{
private:

    pmt::pmt_t d_var_name;

public:
    variable_filter_impl(const std::string& variable_name);
    ~variable_filter_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);

    std::string get_variable_name() const override;
    void set_variable_name(const std::string& s) override;
};

} // namespace UTAT_HERON
} // namespace gr

#endif /* INCLUDED_UTAT_HERON_VARIABLE_FILTER_IMPL_H */
