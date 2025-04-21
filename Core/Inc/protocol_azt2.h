// /Inc/protocol_azt2.h
#ifndef PROTOCOL_AZT2_H
#define PROTOCOL_AZT2_H

#include "protocol.h"

/**
 * Заглушка реализации протокола АЗТ 2.0
 * Сейчас все функции возвращают PROTO_ERROR_GENERAL.
 */

extern const protocol_t Protocol_AZT2;

/**
 * Инициализация протокола AZT 2.0
 * Вызывается один раз при старте, для настройки UART, таймеров и т.п.
 */
void Protocol_AZT2_init(void);

/**
 * Отправка команды протокола AZT 2.0
 * @param frame буфер с формой кадра
 * @param len длина кадра
 * @return статус операции
 */
proto_status_t Protocol_AZT2_send_cmd(const uint8_t *frame, size_t len);

/**
 * Приём ответа протокола AZT 2.0
 * @param frame буфер для приёма кадра
 * @param len [out] полученная длина
 * @param timeout_ms максимальное время ожидания, мс
 * @return статус операции
 */
proto_status_t Protocol_AZT2_recv_resp(uint8_t *frame, size_t *len, uint32_t timeout_ms);

/**
 * Разбор полученного кадра протокола AZT 2.0
 * @param frame буфер с кадром
 * @param len длина кадра
 * @param parsed [out] распарсенная структура (заглушка)
 * @return статус разбора
 */
proto_status_t Protocol_AZT2_parse(const uint8_t *frame, size_t len, void *parsed);

#endif // PROTOCOL_AZT2_H
