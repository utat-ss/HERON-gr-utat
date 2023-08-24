#ifndef INCLUDED_HERON_PACKET_H
#define INCLUDED_HERON_PACKET_H

#include <cstdint>
#include <deque>

struct heron_packet{
    uint32_t preamble;
    uint8_t sync_word;
    uint8_t size_byte;
    std::deque<uint8_t> data;
    uint16_t checksum;
};

#endif