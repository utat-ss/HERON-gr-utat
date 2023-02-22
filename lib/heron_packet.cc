#include "heron_packet.h"
#include <stdexcept>

namespace gr{
namespace UTAT_HERON{

    void heron_packet::clear(){
        preamble = 0x00000000;
        sync_word = 0x00;
        size_byte = 0x00;
        data.clear();
        checksum = 0x0000;
        counter = 0;
    }

    bool heron_packet::preamble_identified(){
        return (preamble & 0xFFFFFFFF) == 0xAAAAAAAA;
    }

    bool heron_packet::sync_word_identified(){
        return (sync_word & 0xFF) == 0x7E;
    }

    bool heron_packet::sync_word_timeout(){
        return counter >= 50;
    }

    bool heron_packet::size_byte_identified(){
        return counter == 8;
    }

    bool heron_packet::data_identified(){
        return counter == 8*size_byte;
    }

    bool heron_packet::checksum_identified(){
        return counter == 16;
    }

    template<typename T>
    void heron_packet::append_bit(uint8_t bit, T& data){
        if (bit == 0x01) {
        data = (data << 1) + 1;
        } else if (bit == 0x00) {
            data = data << 1;
        }else{
            throw std::runtime_error("!!! Don't use Pack K Bits before this block, must be 0x00 or 0x01 input !!!");
        }
    }

    void heron_packet::append_bit_to_preamble(uint8_t bit){
        append_bit(bit, preamble);
    }
    void heron_packet::append_bit_to_sync_word(uint8_t bit){
        append_bit(bit, sync_word);
        counter++;
    }
    void heron_packet::append_bit_to_size_byte(uint8_t bit){
        append_bit(bit, size_byte);
        counter++;
    }
    void heron_packet::append_bit_to_data(uint8_t bit){
        if (counter % 8 == 0) {
            data.push_back(0x00);
        }
        int byte_idx = (int)(counter/8); // Floor counter/8
        append_bit(bit, data[byte_idx]);
        counter++;
    }
    void heron_packet::append_bit_to_checksum(uint8_t bit){
        append_bit(bit, checksum);
        counter++;
    }

}
}