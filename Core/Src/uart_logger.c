#include "uart_logger.h"
#include "usart.h"
#include "stm32f4xx_hal.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define LOG_BUF_SIZE 128

void Logger_Init(void)
{
    // UART2 инициализируется в main перед использованием логгера.
}

void Logger_Log(const char *level, const char *fmt, ...)
{
    char buf[LOG_BUF_SIZE];
    uint32_t tick = HAL_GetTick();
    int idx = snprintf(buf, LOG_BUF_SIZE, "[%05lu] %s: ", (unsigned long)tick, level);
    if (idx < 0) idx = 0;
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf + idx, LOG_BUF_SIZE - idx, fmt, args);
    va_end(args);
    if (n < 0) n = 0;
    size_t len = (size_t)(idx + n);
    if (len > LOG_BUF_SIZE) len = LOG_BUF_SIZE;
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);
}

void UartLogger_Hex(const char *prefix, const uint8_t *data, size_t len)
{
    char line[LOG_BUF_SIZE];
    size_t idx = 0;
    uint32_t tick = HAL_GetTick();
    idx += snprintf(line + idx, sizeof(line) - idx, "[%05lu] %s", (unsigned long)tick, prefix);
    for (size_t i = 0; i < len && idx + 4 < sizeof(line); i++) {
        idx += snprintf(line + idx, sizeof(line) - idx, " %02X", data[i]);
    }
    snprintf(line + idx, sizeof(line) - idx, " \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)line, strlen(line), HAL_MAX_DELAY);
}
