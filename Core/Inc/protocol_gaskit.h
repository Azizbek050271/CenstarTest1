// /Inc/protocol_gaskit.h
#ifndef PROTOCOL_GASKIT_H
#define PROTOCOL_GASKIT_H

#include "protocol.h"
#include "commands_gaskit.h"
#include "uart_trk.h"
#include "app_timer.h"
#include "config.h"

#define GASKIT_MAX_FRAME_SIZE  32

typedef struct {
    uint16_t        address;
    char            cmd;
    const uint8_t  *data;
    size_t          data_len;
} gaskit_parsed_t;

extern const protocol_t Protocol_GasKitLink;

void Protocol_GasKitLink_init(void);
proto_status_t Protocol_GasKitLink_send_cmd(const uint8_t *frame, size_t len);
proto_status_t Protocol_GasKitLink_recv_resp(uint8_t *frame,
                                             size_t *len,
                                             uint32_t timeout_ms);
proto_status_t Protocol_GasKitLink_parse(const uint8_t *frame,
                                         size_t len,
                                         gaskit_parsed_t *parsed);

uint8_t Protocol_GasKitLink_build_command(
    uint8_t *dst,
    char cmd,
    const char *data,
    uint8_t data_len,
    uint8_t post_addr);

#endif // PROTOCOL_GASKIT_H
