#include "pdu_lambda.h"

namespace gr{
namespace UTAT_HERON{
namespace utils{

pdu_lambda::sptr pdu_lambda::make(pdu_lambda::func callback){
    return gnuradio::make_block_sptr<pdu_lambda>(callback);
}

pdu_lambda::pdu_lambda(pdu_lambda::func callback):
    gr::block(
        "pdu_lambda",
        gr::io_signature::make(0,0,0),
        gr::io_signature::make(0,0,0)
    ),
    callback(callback)
{
    message_port_register_in(pmt::mp("pdu_in"));
    message_port_register_out(pmt::mp("pdu_out"));

    set_msg_handler(pmt::mp("pdu_in"), [this](pmt::pmt_t pdu){
        pmt::pmt_t new_pdu = this->callback(pdu);
        message_port_pub(pmt::mp("pdu_out"), new_pdu);
    });
}

int pdu_lambda::general_work(
    int noutput_items,
    gr_vector_int &ninput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items
){return 0;}

void pdu_lambda::forecast(
    int noutput_items,
    gr_vector_int &ninput_items_required
){}

}
}
}