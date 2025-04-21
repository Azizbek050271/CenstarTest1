// /Inc/uart_trk.h
#ifndef UART_TRK_H
#define UART_TRK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Инициализация UART3 для обмена с ТРК и кольцевого буфера приёма.
 *        Вызывать один раз при старте.
 */
void TrkUart_Init(void);

/**
 * @brief Отправить буфер через UART3.
 * @param data указатель на данные
 * @param len  длина буфера в байтах
 */
void TrkUart_Send(const uint8_t *data, size_t len);

/**
 * @brief Прочитать один принятый байт из кольцевого буфера.
 * @param out_byte [out] прочитанный байт
 * @return true, если байт был доступен, false если буфер пуст
 */
bool TrkUart_ReadByte(uint8_t *out_byte);

#endif // UART_TRK_H
