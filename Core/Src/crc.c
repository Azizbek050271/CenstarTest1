// /Src/crc.c
#include "crc.h"

/**
 * Поліном  CRC-16/ARC: 0xA001 (відображення поліному 0x8005)
 * Ініціалізація: 0xFFFF
 * Рефлексія даних і результату: так
 */
uint16_t CRC16_Calc(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}
