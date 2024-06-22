#include <cstdint>
#include <gnuradio/UTAT_HERON/header_format_esttc.h>

namespace gr{
namespace UTAT_HERON{
namespace utils{
namespace crc{

    constexpr int num_bits = 16;
    constexpr uint64_t poly = 0x1021;
    constexpr uint64_t inital_value = 0xFFFF;
    constexpr uint64_t final_xor = 0x0000;
    constexpr bool input_reflected = false;
    constexpr bool result_reflected = false;

}

extern const char* access_code;
constexpr int access_code_front_trim = 8*3+2;
extern const char* trimmed_access_code;

}
}
}
