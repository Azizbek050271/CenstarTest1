// /Inc/protocol_gaskit.h
#ifndef PROTOCOL_GASKIT_H
#define PROTOCOL_GASKIT_H

#include "protocol.h"
#include "commands_gaskit.h"
#include "uart_trk.h"
#include "app_timer.h"
#include "config.h"

/** Максимальный размер кадра GasKitLink */
#define GASKIT_MAX_FRAME_SIZE  32

/** Структура разобранного кадра GasKitLink */
typedef struct {
    uint16_t        address;    ///< адрес ведомой станции
    char            cmd;        ///< командный символ
    const uint8_t  *data;       ///< указатель на область данных
    size_t          data_len;   ///< длина данных
} gaskit_parsed_t;

/** Экземпляр протокола GasKitLink */
extern const protocol_t Protocol_GasKitLink;

/** Инициализация протокола (UART3, таймеры) */
void Protocol_GasKitLink_init(void);

/** Отправить команду */
proto_status_t Protocol_GasKitLink_send_cmd(const uint8_t *frame, size_t len);

/** Получить ответ (кадр) */
proto_status_t Protocol_GasKitLink_recv_resp(uint8_t *frame,
                                             size_t *len,
                                             uint32_t timeout_ms);

/** Разобрать кадр */
proto_status_t Protocol_GasKitLink_parse(const uint8_t *frame,
                                         size_t len,
                                         gaskit_parsed_t *parsed);

#endif // PROTOCOL_GASKIT_H
