// /Src/protocol_azt2.c
#include "protocol_azt2.h"
#include "uart_logger.h"  // для логирования заглушек

const protocol_t Protocol_AZT2 = {
    .init      = Protocol_AZT2_init,
    .send_cmd  = Protocol_AZT2_send_cmd,
    .recv_resp = Protocol_AZT2_recv_resp,
    .parse     = Protocol_AZT2_parse
};

void Protocol_AZT2_init(void)
{
    // Заглушка: будущая инициализация UART / таймеров для AZT 2.0
    LOG_INFO("AZT2 protocol init (stub)");
}

proto_status_t Protocol_AZT2_send_cmd(const uint8_t *frame, size_t len)
{
    (void)frame; (void)len;
    LOG_ERROR("AZT2 send_cmd stub called");
    return PROTO_ERROR_GENERAL;
}

proto_status_t Protocol_AZT2_recv_resp(uint8_t *frame, size_t *len, uint32_t timeout_ms)
{
    (void)frame; (void)len; (void)timeout_ms;
    LOG_ERROR("AZT2 recv_resp stub called");
    return PROTO_ERROR_GENERAL;
}

proto_status_t Protocol_AZT2_parse(const uint8_t *frame, size_t len, void *parsed)
{
    (void)frame; (void)len; (void)parsed;
    LOG_ERROR("AZT2 parse stub called");
    return PROTO_ERROR_GENERAL;
}
