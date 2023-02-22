#include "heron_packet_manager.h"
#include <stdexcept>

namespace gr{
namespace UTAT_HERON{

    void heron_packet_manager::clear(){
        preamble = 0x00000000;
        sync_word = 0x00;
        size_byte = 0x00;
        data.clear();
        checksum = 0x0000;
        counter = 0;
        state = getting_preamble;
    }

    bool heron_packet_manager::preamble_identified(){
        return (preamble & 0xFFFFFFFF) == 0xAAAAAAAA;
    }

    bool heron_packet_manager::sync_word_identified(){
        return (sync_word & 0xFF) == 0x7E;
    }

    bool heron_packet_manager::sync_word_timeout(){
        return counter >= 50;
    }

    bool heron_packet_manager::size_byte_identified(){
        return counter == 8;
    }

    bool heron_packet_manager::data_identified(){
        return counter == 8*size_byte;
    }

    bool heron_packet_manager::checksum_identified(){
        return counter == 16;
    }

    template<typename T>
    void heron_packet_manager::append_bit(uint8_t bit, T& data){
        if (bit == 0x01) {
        data = (data << 1) + 1;
        } else if (bit == 0x00) {
            data = data << 1;
        }else{
            throw std::runtime_error("!!! Don't use Pack K Bits before this block, must be 0x00 or 0x01 input !!!");
        }
    }

    void heron_packet_manager::append_bit_to_preamble(uint8_t bit){
        append_bit(bit, preamble);
    }
    void heron_packet_manager::append_bit_to_sync_word(uint8_t bit){
        append_bit(bit, sync_word);
        counter++;
    }
    void heron_packet_manager::append_bit_to_size_byte(uint8_t bit){
        append_bit(bit, size_byte);
        counter++;
    }
    void heron_packet_manager::append_bit_to_data(uint8_t bit){
        if (counter % 8 == 0) {
            data.push_back(0x00);
        }
        int byte_idx = (int)(counter/8); // Floor counter/8
        append_bit(bit, data[byte_idx]);
        counter++;
    }
    void heron_packet_manager::append_bit_to_checksum(uint8_t bit){
        append_bit(bit, checksum);
        counter++;
    }

    uint8_t heron_packet_manager::pop_data_if_avail(){
        uint8_t ret = data.front();
        data.pop_front();
        if(data.empty()) this->clear();
        return ret;
    }

    uint16_t
    heron_packet_manager::crc16() const
    {
        // Uses the "CRC16/CCITT-FALSE implementation from https://crccalc.com
        uint16_t poly = 0x1021;
        uint16_t data_byte = 0x0000;
        uint16_t crc = 0xFFFF;

        /* calculate value with data_length as input */
        data_byte = size_byte;
        data_byte <<= 8;
        for(int i=0; i<8; i++) {
            uint16_t xor_flag = (crc ^ data_byte) & 0x8000;
            crc <<= 1;
            if(xor_flag) {
                crc ^= poly;
            }
            data_byte <<= 1;
        }

        /* calculate crc value for the rest of the datafields */
        for(size_t i=0; i < data.size(); i++){
            data_byte = data[i];
            data_byte <<= 8;
            for(int i=0; i<8; i++) {
                uint16_t xor_flag = (crc ^ data_byte) & 0x8000;
                crc <<= 1;
                if(xor_flag) {
                    crc ^= poly;
                }
                data_byte <<= 1;
            }
        }

        /* return crc_value */
        return crc;
    }

    bool heron_packet_manager::checksum_matches() const{
        return crc16() == checksum;
    }

}
}