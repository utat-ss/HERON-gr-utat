
#include <gnuradio/block.h>
#include <functional>

namespace gr{
namespace UTAT_HERON{
namespace utils{

class pdu_lambda : public gr::block{
private:
    typedef std::function<pmt::pmt_t(pmt::pmt_t)> func;
    func callback;
public:

    pdu_lambda(func callback);

    int general_work(
        int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items
    ) override;

    void forecast(
        int noutput_items,
        gr_vector_int &ninput_items_required
    ) override;

    typedef std::shared_ptr<pdu_lambda> sptr;
    static sptr make(func callback);
};

}
}
}