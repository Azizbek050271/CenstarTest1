// /Inc/protocol.h
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    PROTO_OK = 0,
    PROTO_ERROR_TIMEOUT,
    PROTO_ERROR_CRC,
    PROTO_ERROR_OVERFLOW,
    PROTO_ERROR_GENERAL
} proto_status_t;

typedef struct {
    void            (*init)(void);
    proto_status_t  (*send_cmd)(const uint8_t *frame, size_t len);
    proto_status_t  (*recv_resp)(uint8_t *frame, size_t *len, uint32_t timeout_ms);
    proto_status_t  (*parse)(const uint8_t *frame, size_t len, void *parsed);
} protocol_t;

#endif // PROTOCOL_H
