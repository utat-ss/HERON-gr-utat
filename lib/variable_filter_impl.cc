/* -*- c++ -*- */
/*
 * Copyright 2023 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "variable_filter_impl.h"
#include <gnuradio/io_signature.h>
#include <functional>

namespace gr {
namespace UTAT_HERON {

variable_filter::sptr variable_filter::make(const std::string& variable_name)
{
    return gnuradio::make_block_sptr<variable_filter_impl>(variable_name);
}


/*
 * The private constructor
 */
variable_filter_impl::variable_filter_impl(const std::string& variable_name)
    : gr::block("variable_filter",
                gr::io_signature::make(0,0,0),
                gr::io_signature::make(0,0,0)),
    d_var_name(pmt::intern(variable_name))
{
    auto in_port = pmt::intern("in");
    auto out_port = pmt::intern("out");
    message_port_register_in(in_port);
    message_port_register_out(out_port);
    set_msg_handler(in_port, [&](const pmt::pmt_t& msg){
        try{
            if(pmt::eqv(d_var_name, pmt::car(msg)))
                message_port_pub(out_port, msg);
        }catch(pmt::wrong_type&){
            d_logger->alert("bad variable message");
        }
    });
}

/*
 * Our virtual destructor.
 */
variable_filter_impl::~variable_filter_impl() {}

void variable_filter_impl::forecast(int noutput_items,
                                    gr_vector_int& ninput_items_required)
{}

int variable_filter_impl::general_work(int noutput_items,
                                       gr_vector_int& ninput_items,
                                       gr_vector_const_void_star& input_items,
                                       gr_vector_void_star& output_items)
{
    return 0;
}

std::string variable_filter_impl::get_variable_name() const { return pmt::symbol_to_string(d_var_name); }
void variable_filter_impl::set_variable_name(const std::string& s) { d_var_name = pmt::intern(s); }

} /* namespace UTAT_HERON */
} /* namespace gr */
