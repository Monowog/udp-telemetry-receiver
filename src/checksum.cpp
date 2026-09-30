#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <array>
#include "udprx/checksum.hpp"

std::array<uint32_t, 256> make_table(){
    std::array<uint32_t, 256> table = std::array<uint32_t, 256>{0};
    uint32_t poly = 0xEDB88320;

    for(int i = 0; i<256; i++){
        uint32_t crc = i;

        for(int j = 0; j <=7; j++){
            if (crc & 1){
                crc = (crc >> 1) ^ poly;
            } else {
                crc = (crc >> 1);
            }
        }
        table[i] = crc;
    }

    return table;
}

uint32_t crc32(const uint8_t* data, size_t length){
    static std::array<uint32_t, 256> table = make_table();

    uint32_t crc = 0xFFFFFFFF;

    for(int i = 0; i < length; i++){
        crc = (crc >> 8) ^ table[(crc ^ data[i]) & 0xFF];
    }

    return (crc ^ 0xFFFFFFFF);
}

