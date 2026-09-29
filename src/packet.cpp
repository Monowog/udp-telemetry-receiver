#include <arpa/inet.h>
#include <string.h>
#include "udprx/packet.hpp"

std::array<uint8_t, WIRE_SIZE> serialize(const Packet& packet) {
    std::array<uint8_t, WIRE_SIZE> data;

    uint16_t magic = htons(packet.magic);
    memcpy(&data[0], &magic, sizeof(magic));
    
    memcpy(&data[2], &packet.version, sizeof(packet.version));

    memcpy(&data[3], &packet.sensor_type, sizeof(packet.sensor_type));

    uint32_t sequence_number = htonl(packet.sequence_number);
    memcpy(&data[4], &sequence_number, sizeof(sequence_number));

    uint32_t timestamp = htonl(packet.timestamp);
    memcpy(&data[8], &timestamp, sizeof(timestamp));

    for(int i = 0; i <= 2; i++){
        uint32_t temp = 0;
        memcpy(&temp, &packet.values[i], sizeof(temp));
        temp = htonl(temp);
        memcpy(&data[12+(i*4)], &temp, sizeof(temp));
    }

    uint32_t checksum = htonl(packet.checksum);
    memcpy(&data[24], &checksum, sizeof(checksum));

    return data;
}

Packet deserialize(std::array<uint8_t, WIRE_SIZE> data) {
    Packet packet;

    memcpy(&packet.magic, &data[0], sizeof(packet.magic));
    packet.magic = ntohs(packet.magic);

    memcpy(&packet.version, &data[2], sizeof(packet.version));

    memcpy(&packet.sensor_type, &data[3], sizeof(packet.sensor_type));

    memcpy(&packet.sequence_number, &data[4], sizeof(packet.sequence_number));
    packet.sequence_number = ntohl(packet.sequence_number);

    memcpy(&packet.timestamp, &data[8], sizeof(packet.timestamp));
    packet.timestamp = ntohl(packet.timestamp);

    for(int i = 0; i<=2; i++){
        uint32_t temp = 0;
        memcpy(&temp, &data[12+(i*4)], sizeof(packet.values[i]));
        temp = ntohl(temp);
        memcpy(&packet.values[i], &temp, sizeof(packet.values[i]));
    }

    memcpy(&packet.checksum, &data[24], sizeof(packet.checksum));
    packet.checksum = ntohl(packet.checksum);

    return packet;
}