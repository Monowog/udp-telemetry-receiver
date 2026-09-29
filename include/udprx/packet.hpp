/*
-- Packet structure for UDP telemetry data --
magic: 2-byte identifier constant
version: 1-byte version number
sensor_type: 1-byte sensor type identifier (GPS=0, IMU=1)
sequence_number: 4-byte monotonically increasing value denoting relative order of packets
timestamp: 4-byte timestamp indicating time elapsed since sender started (ms)
values[3]: array of 3 floating-point values representing sensor readings
checksum: 4-byte checksum for error detection
    
*/

#include <stdint.h>
#include <array>

const int WIRE_SIZE = 28;

struct Packet {
    uint16_t magic;
    uint8_t version;
    uint8_t sensor_type;
    uint32_t sequence_number;
    uint32_t timestamp;
    float values[3];
    uint32_t checksum;
};

std::array<uint8_t, WIRE_SIZE> serialize(const Packet& packet);

Packet deserialize(std::array<uint8_t, WIRE_SIZE> data);