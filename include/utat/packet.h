#ifndef HERON_PACKET_FORMAT_H
#define HERON_PACKET_FORMAT_H

#include <vector>

typedef struct heron_packet_type {
    uint32_t preamble;
    uint8_t sync_word;
    uint8_t size_byte;
    std::vector<uint8_t> data;
    uint16_t checksum;
} heron_packet;

#endif // HERON_PACKET_FORMAT_H
