#ifndef INCLUDED_UTAT_HERON_HERON_PACKET_H
#define INCLUDED_UTAT_HERON_HERON_PACKET_H

#include <cstdint>
#include <deque>

namespace gr {
namespace UTAT_HERON {

class heron_packet {
private:

    template<typename T>
    void append_bit(uint8_t bit, T& data);

public:
    uint32_t preamble;
    uint8_t sync_word;
    uint8_t size_byte;
    std::deque<uint8_t> data;
    uint16_t checksum;
    int counter;

    void clear();
    bool preamble_identified();
    bool sync_word_identified();
    bool size_byte_identified();
    bool data_identified();
    bool checksum_identified();
    void append_bit_to_preamble(uint8_t bit);
    void append_bit_to_sync_word(uint8_t bit);
    void append_bit_to_size_byte(uint8_t bit);
    void append_bit_to_data(uint8_t bit);
    void append_bit_to_checksum(uint8_t bit);
    bool sync_word_timeout();
};

}
}

#endif