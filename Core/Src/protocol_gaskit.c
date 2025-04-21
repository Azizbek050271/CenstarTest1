// /Src/protocol_gaskit.c
#include "protocol_gaskit.h"
#include <string.h>
#include "commands_gaskit.h"
#include "uart_trk.h"
#include "app_timer.h"
#include "config.h"
#include "crc.h"          // если понадобится для расширенного CRC

const protocol_t Protocol_GasKitLink = {
    .init      = Protocol_GasKitLink_init,
    .send_cmd  = Protocol_GasKitLink_send_cmd,
    .recv_resp = Protocol_GasKitLink_recv_resp,
    .parse     = (proto_status_t(*)(const uint8_t*,size_t,void*))Protocol_GasKitLink_parse
};

void Protocol_GasKitLink_init(void)
{
    // Настроить UART3 для обмена с ТРК и таймеры
    TrkUart_Init();
    for (int i = 0; i < APP_TIMER_COUNT; i++) {
        AppTimer_Stop(i);
    }
}

proto_status_t Protocol_GasKitLink_send_cmd(const uint8_t *frame, size_t len)
{
    // передаём кадр по UART3
    TrkUart_Send(frame, len);
    return PROTO_OK;
}

proto_status_t Protocol_GasKitLink_recv_resp(uint8_t *frame,
                                             size_t *len,
                                             uint32_t timeout_ms)
{
    size_t idx = 0;
    uint8_t byte;

    // Запустить таймер ответа
    AppTimer_Start(TIMER_PROTO_RESPONSE, timeout_ms, false);

    while (1) {
        // Проверить общий таймаут
        if (AppTimer_Expired(TIMER_PROTO_RESPONSE)) {
            return PROTO_ERROR_TIMEOUT;
        }
        // Попробовать прочитать байт из кольцевого буфера
        if (TrkUart_ReadByte(&byte)) {
            if (idx == 0) {
                // Ищем стартовый байт
                if (byte != GASKIT_START_BYTE) {
                    continue;
                }
                frame[idx++] = byte;
                // Запустить межкадровый таймаут
                AppTimer_Start(TIMER_PROTO_INTERFRAME,
                               PROTO_INTERFRAME_TIMEOUT_MS,
                               false);
            } else {
                // Сохраняем следующий байт
                if (idx >= GASKIT_MAX_FRAME_SIZE) {
                    return PROTO_ERROR_OVERFLOW;
                }
                frame[idx++] = byte;
                // Перезапустить межкадровый таймаут
                AppTimer_Start(TIMER_PROTO_INTERFRAME,
                               PROTO_INTERFRAME_TIMEOUT_MS,
                               false);
                // Проверяем контрольную сумму, если минимум 5 байт
                if (idx >= 5) {
                    uint8_t cs = Gaskit_CalcChecksum(frame, idx);
                    if (cs == frame[idx - 1]) {
                        *len = idx;
                        return PROTO_OK;
                    }
                }
            }
        }
        // Межкадровый таймаут — сбросить на поиск нового кадра
        if (idx > 0 && AppTimer_Expired(TIMER_PROTO_INTERFRAME)) {
            idx = 0;
            AppTimer_Stop(TIMER_PROTO_INTERFRAME);
        }
    }
}

proto_status_t Protocol_GasKitLink_parse(const uint8_t *frame,
                                         size_t len,
                                         gaskit_parsed_t *parsed)
{
    if (len < 5 || frame[0] != GASKIT_START_BYTE) {
        return PROTO_ERROR_CRC;
    }
    // Проверка CRC
    uint8_t cs = Gaskit_CalcChecksum(frame, len);
    if (cs != frame[len - 1]) {
        return PROTO_ERROR_CRC;
    }
    // Распарсить поля
    parsed->address  = (uint16_t)frame[1] << 8 | frame[2];
    parsed->cmd      = (char)frame[3];
    parsed->data     = &frame[4];
    parsed->data_len = len - 5;
    return PROTO_OK;
}
