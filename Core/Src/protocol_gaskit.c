// /Src/protocol_gaskit.c
#include "protocol_gaskit.h"
#include <string.h>
#include "commands_gaskit.h"
#include "uart_trk.h"
#include "app_timer.h"
#include "config.h"
#include "crc.h"

const protocol_t Protocol_GasKitLink = {
    .init      = Protocol_GasKitLink_init,
    .send_cmd  = Protocol_GasKitLink_send_cmd,
    .recv_resp = Protocol_GasKitLink_recv_resp,
    .parse     = (proto_status_t(*)(const uint8_t*,size_t,void*))Protocol_GasKitLink_parse
};

void Protocol_GasKitLink_init(void)
{
    TrkUart_Init();
    for (int i = 0; i < APP_TIMER_COUNT; i++) {
        AppTimer_Stop(i);
    }
}

proto_status_t Protocol_GasKitLink_send_cmd(const uint8_t *frame, size_t len)
{
    TrkUart_Send(frame, len);
    return PROTO_OK;
}

proto_status_t Protocol_GasKitLink_recv_resp(uint8_t *frame,
                                             size_t *len,
                                             uint32_t timeout_ms)
{
    size_t idx = 0;
    uint8_t byte;
    AppTimer_Start(TIMER_PROTO_RESPONSE, timeout_ms, false);

    while (1) {
        if (AppTimer_Expired(TIMER_PROTO_RESPONSE)) {
            return PROTO_ERROR_TIMEOUT;
        }
        if (TrkUart_ReadByte(&byte)) {
            if (idx == 0) {
                if (byte != GASKIT_START_BYTE) continue;
                frame[idx++] = byte;
                AppTimer_Start(TIMER_PROTO_INTERFRAME, PROTO_INTERFRAME_TIMEOUT_MS, false);
            } else {
                if (idx >= GASKIT_MAX_FRAME_SIZE) return PROTO_ERROR_OVERFLOW;
                frame[idx++] = byte;
                AppTimer_Start(TIMER_PROTO_INTERFRAME, PROTO_INTERFRAME_TIMEOUT_MS, false);
                if (idx >= 5) {
                    uint8_t cs = Gaskit_CalcChecksum(frame, idx);
                    if (cs == frame[idx - 1]) {
                        *len = idx;
                        return PROTO_OK;
                    }
                }
            }
        }
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
    uint8_t cs = Gaskit_CalcChecksum(frame, len);
    if (cs != frame[len - 1]) {
        return PROTO_ERROR_CRC;
    }
    parsed->address  = (uint16_t)frame[1] << 8 | frame[2];
    parsed->cmd      = (char)frame[3];
    parsed->data     = &frame[4];
    parsed->data_len = len - 5;
    return PROTO_OK;
}

uint8_t Protocol_GasKitLink_build_command(
    uint8_t *dst,
    char cmd,
    const char *data,
    uint8_t data_len,
    uint8_t post_addr)
{
    uint8_t i = 0;
    dst[i++] = GASKIT_START_BYTE;
    dst[i++] = 0x00;
    dst[i++] = post_addr;
    dst[i++] = (uint8_t)cmd;
    for (uint8_t j = 0; j < data_len; j++) {
        dst[i++] = (uint8_t)data[j];
    }
    dst[i++] = Gaskit_CalcChecksum(dst, i);
    return i;
}
