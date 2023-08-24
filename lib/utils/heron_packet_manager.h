#ifndef INCLUDED_HERON_PACKET_MANAGER_H
#define INCLUDED_HERON_PACKET_MANAGER_H

#include "heron_packet.h"

enum rx_recog_state {
    getting_preamble,
    getting_sync_word,
    getting_size_byte,
    getting_data,
    getting_checksum,
    finished_packet,
};

class heron_packet_manager {
private:

    heron_packet packet;
    int counter;
    rx_recog_state state;

    template<typename T>
    void append_bit(uint8_t bit, T& data);
    void append_bit_to_preamble(uint8_t bit);
    void append_bit_to_sync_word(uint8_t bit);
    void append_bit_to_size_byte(uint8_t bit);
    void append_bit_to_data(uint8_t bit);
    void append_bit_to_checksum(uint8_t bit);

    bool preamble_identified() const;
    bool sync_word_identified() const;
    bool size_byte_identified() const;
    bool data_identified() const;
    bool checksum_identified() const;

    void move_to_getting_preamble();
    void move_to_getting_sync_word();
    void move_to_getting_size_byte();
    void move_to_getting_data();
    void move_to_getting_checksum();
    void notify_data_available();

    void process_preamble_bit(uint8_t bit);
    void process_sync_word_bit(uint8_t bit);
    void process_size_byte_bit(uint8_t bit);
    void process_data_bit(uint8_t bit);
    void process_checksum_bit(uint8_t bit);
    void process_finished_data();

    void debug_log_getting_preamble() const;
    void debug_log_getting_sync_word() const;
    void debug_log_getting_size_byte() const;
    void debug_log_getting_data() const;
    void debug_log_getting_checksum() const;

    void debug_log_finished_preamble() const;
    void debug_log_finished_sync_word() const;
    void debug_log_sync_word_timeout() const;
    void debug_log_finished_size_byte() const;
    void debug_log_finished_data() const;
    void debug_log_finished_checksum() const;

    void debug_log_summarize_data() const;

    void clear();
    bool sync_word_timeout() const;
    uint16_t crc16() const;
    bool checksum_matches() const;
    void handle_invalid_checksum();

public:

    heron_packet_manager();

    void process_bit(uint8_t bit);
    uint8_t pop_data();
    bool data_available();
};

#endif