// /Inc/crc.h
#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Вычисление CRC путём XOR всех байт (GasKitLink v1.2)
 * @param data Указатель на байты, начиная со второго
 * @param len  Длина (в байтах) от 3-го байта и далее
 * @return Результат XOR, соответствующий ASCII-символу
 */
uint8_t CRC_XOR_Calc(const uint8_t *data, size_t len);

#endif // CRC_H
