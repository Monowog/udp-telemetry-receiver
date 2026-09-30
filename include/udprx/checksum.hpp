#include <stdint.h>
#include <stdlib.h>
#include <array>

std::array<uint32_t, 256> make_table();

uint32_t crc32(const uint8_t* data, size_t length);