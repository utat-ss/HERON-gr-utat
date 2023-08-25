
#include <gnuradio/sync_block.h>
#include <functional>

namespace gr{
namespace UTAT_HERON{
namespace utils{

class pdu_lambda : public gr::sync_block{
private:
    typedef std::function<void(pmt::pmt_t)> func;
    func callback;
public:

    pdu_lambda(func callback);

    int work(
        int noutput_items,
        gr_vector_const_void_star& input_items,
        gr_vector_void_star& output_items
    ) override;

    typedef std::shared_ptr<pdu_lambda> sptr;
    static sptr make(func callback);
};

}
}
}