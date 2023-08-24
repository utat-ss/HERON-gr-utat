#include "heron_packet_manager.h"
#include "debug_logger.h"
#include <stdexcept>
#include <iomanip>

heron_packet_manager::heron_packet_manager(){
    move_to_getting_preamble();
}


bool heron_packet_manager::preamble_identified() const{
    return (packet.preamble & 0xFFFFFFFF) == 0xAAAAAAAA;
}
bool heron_packet_manager::sync_word_identified() const{
    return (packet.sync_word & 0xFF) == 0x7E;
}
bool heron_packet_manager::size_byte_identified() const{
    return counter == 8;
}
bool heron_packet_manager::data_identified() const{
    return counter == 8*packet.size_byte;
}
bool heron_packet_manager::checksum_identified() const{
    return counter == 16;
}


void heron_packet_manager::move_to_getting_preamble(){
    
    packet.preamble = 0x00000000;
    packet.sync_word = 0x00;
    packet.size_byte = 0x00;
    packet.data.clear();
    packet.checksum = 0x0000;

    state = getting_preamble;
}
void heron_packet_manager::move_to_getting_sync_word(){
    state = getting_sync_word;
    counter = 0;
}
void heron_packet_manager::move_to_getting_size_byte(){
    state = getting_size_byte;
    counter = 0;
}
void heron_packet_manager::move_to_getting_data(){
    state = getting_data;
    counter = 0;
}
void heron_packet_manager::move_to_getting_checksum(){
    state = getting_checksum;
    counter = 0;
}
void heron_packet_manager::notify_data_available(){
    state = finished_packet;
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
    append_bit(bit, packet.preamble);
}
void heron_packet_manager::append_bit_to_sync_word(uint8_t bit){
    append_bit(bit, packet.sync_word);
    counter++;
}
void heron_packet_manager::append_bit_to_size_byte(uint8_t bit){
    append_bit(bit, packet.size_byte);
    counter++;
}
void heron_packet_manager::append_bit_to_data(uint8_t bit){
    if (counter % 8 == 0) {
        packet.data.push_back(0x00);
    }
    int byte_idx = (int)(counter/8); // Floor counter/8
    append_bit(bit, packet.data[byte_idx]);
    counter++;
}
void heron_packet_manager::append_bit_to_checksum(uint8_t bit){
    append_bit(bit, packet.checksum);
    counter++;
}

uint8_t heron_packet_manager::pop_data(){
    uint8_t ret = packet.data.front();
    packet.data.pop_front();
    if(packet.data.empty()) move_to_getting_preamble();
    return ret;
}

bool heron_packet_manager::data_available(){
    return state == finished_packet;
}

uint16_t
heron_packet_manager::crc16() const
{
    // Uses the "CRC16/CCITT-FALSE implementation from https://crccalc.com
    uint16_t poly = 0x1021;
    uint16_t data_byte = 0x0000;
    uint16_t crc = 0xFFFF;

    /* calculate value with data_length as input */
    data_byte = packet.size_byte;
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
    for(size_t i=0; i < packet.data.size(); i++){
        data_byte = packet.data[i];
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
    return crc16() == packet.checksum;
}

void heron_packet_manager::process_bit(uint8_t bit){

    switch (state) {

    case getting_preamble:
        process_preamble_bit(bit);
        break;

    case getting_sync_word:
        process_sync_word_bit(bit);
        break;

    case getting_size_byte:
        process_size_byte_bit(bit);
        break;

    case getting_data:
        process_data_bit(bit);
        break;

    case getting_checksum:
        process_checksum_bit(bit);
        break;
    
    case finished_packet:
        throw std::runtime_error("packet is ready! please read it before processing new bits");

    }
}

void heron_packet_manager::process_preamble_bit(uint8_t bit){
    append_bit_to_preamble(bit);
    debug_log_getting_preamble();
    if (preamble_identified()) {
        move_to_getting_sync_word();
        debug_log_finished_preamble();
    }
}

void heron_packet_manager::process_sync_word_bit(uint8_t bit){
    append_bit_to_sync_word(bit);
    debug_log_getting_sync_word();
    if(sync_word_identified()){
        move_to_getting_size_byte();
        debug_log_finished_sync_word();
    }else if(sync_word_timeout()){
        debug_log_sync_word_timeout();
        move_to_getting_preamble();
    }
}

bool heron_packet_manager::sync_word_timeout() const{
    return counter >= 50;
}

void heron_packet_manager::process_size_byte_bit(uint8_t bit){
    append_bit_to_size_byte(bit);
    debug_log_getting_size_byte();
    if(size_byte_identified()){
        if(packet.size_byte == 0) move_to_getting_checksum();
        else move_to_getting_data();
        debug_log_finished_size_byte();
    }
}

void heron_packet_manager::process_data_bit(uint8_t bit){
    append_bit_to_data(bit);
    debug_log_getting_data();
    if(data_identified()){
        move_to_getting_checksum();
        debug_log_finished_data();
    }
}

void heron_packet_manager::process_checksum_bit(uint8_t bit){

    append_bit_to_checksum(bit);
    debug_log_getting_checksum();
    if(checksum_identified()){
        debug_log_finished_checksum();
        if(checksum_matches())
            process_finished_data();
        else
            handle_invalid_checksum();
    }
}

void heron_packet_manager::process_finished_data(){
    debug_log_summarize_data();
    // append carriage return for ESTTC protocol
    packet.data.push_back(0x0D);
    notify_data_available();
}

void heron_packet_manager::handle_invalid_checksum(){
    DEBUG_STREAM("PACKET RECEIVED - INVALID CHECKSUM\n");
    move_to_getting_preamble();
}

void heron_packet_manager::debug_log_getting_preamble() const{
    DEBUG_STREAM("getting preamble: 0x" << packet.preamble << '\n');
}
void heron_packet_manager::debug_log_getting_sync_word() const{
    DEBUG_STREAM("getting sync word: 0x" << (int)(packet.sync_word) << '\n');
}
void heron_packet_manager::debug_log_getting_size_byte() const{
    DEBUG_STREAM("getting size byte: 0x" << (int)(packet.size_byte) << '\n');
}
void heron_packet_manager::debug_log_getting_data() const{
    DEBUG_STREAM("getting data\n");
}
void heron_packet_manager::debug_log_getting_checksum() const{
    DEBUG_STREAM("getting checksum: 0x" << packet.checksum << '\n');
}

void heron_packet_manager::debug_log_finished_preamble() const{
    DEBUG_STREAM(" ::: Finished getting preamble ::: " << std::endl);
}
void heron_packet_manager::debug_log_finished_sync_word() const{
    DEBUG_STREAM(" ::: Finished getting sync word ::: " << std::endl);
}
void heron_packet_manager::debug_log_sync_word_timeout() const{
    DEBUG_STREAM(" !!! Sync word timeout !!! " << std::endl);
}
void heron_packet_manager::debug_log_finished_size_byte() const{
    DEBUG_STREAM(std::dec);
    DEBUG_STREAM(" ::: Finished getting size byte: " << (int)(packet.size_byte) << " bytes expected ::: " << std::endl);
    DEBUG_STREAM(std::hex);
}
void heron_packet_manager::debug_log_finished_data() const{
    DEBUG_STREAM(" ::: Finished getting data ::: " << std::endl);
}
void heron_packet_manager::debug_log_finished_checksum() const{
    DEBUG_STREAM(" ::: Finished getting checksum ::: " << std::endl);
}

void heron_packet_manager::debug_log_summarize_data() const{
    #ifdef DEBUG_LOGGER
    debug_logger << "PACKET RECEIVED - VALID CHECKSUM\n";
    debug_logger << "Packet length: " << std::to_string(packet.size_byte) << std::endl;
    debug_logger << "Packet contents (HEX): " << std::endl;
    for (uint8_t j = 0; j < packet.size_byte; j++) {
        debug_logger << "0x" << std::setfill('0') << std::setw(2) << (int)(packet.data[j]) << " ";
    }
    debug_logger << std::endl;
    debug_logger << "Packet contents (regular text): " << std::endl;
    for (uint8_t j = 0; j < packet.size_byte; j++) {
        debug_logger << packet.data[j] << " ";
    }
    debug_logger << std::endl;
    debug_logger << "================================" << std::endl;
    #endif
}
