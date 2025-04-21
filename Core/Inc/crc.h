// /Inc/crc.h
#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief  Вираховує CRC-16 (поліном 0xA001) для заданого буфера даних.
 * @param  data Вказівник на вхідні дані
 * @param  len  Кількість байт у буфері
 * @return Обчислене значення CRC16
 */
uint16_t CRC16_Calc(const uint8_t *data, size_t len);

#endif // CRC_H
