// /Src/crc.c
#include "crc.h"

uint8_t CRC_XOR_Calc(const uint8_t *data, size_t len)
{
    if (len == 0) return 0;

    uint8_t crc = data[0];
    for (size_t i = 1; i < len; i++) {
        crc ^= data[i];
    }
    return crc;
}
