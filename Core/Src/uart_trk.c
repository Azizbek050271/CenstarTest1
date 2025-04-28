// /Src/uart_trk.c

#include "uart_trk.h"
#include "config.h"       // для TRK_RX_BUF_SIZE
#include "usart.h"        // extern UART_HandleTypeDef huart3
#include "stm32f4xx_hal.h"
#include "uart_logger.h"  // для логов UART_RX

static uint8_t  trk_rx_buf[TRK_RX_BUF_SIZE];
static volatile uint16_t trk_rx_head = 0;
static volatile uint16_t trk_rx_tail = 0;

/**
 * @brief  Перезапустить приём одного байта в кольцевой буфер.
 */
static void TrkUart_StartRxIT(void)
{
    HAL_UART_Receive_IT(&huart3, &trk_rx_buf[trk_rx_head], 1);
}

void TrkUart_Init(void)
{
    trk_rx_head = trk_rx_tail = 0;
    TrkUart_StartRxIT();
}

void TrkUart_Send(const uint8_t *data, size_t len)
{
    HAL_UART_Transmit(&huart3, (uint8_t*)data, len, HAL_MAX_DELAY);
}

bool TrkUart_ReadByte(uint8_t *out_byte)
{
    if (trk_rx_tail == trk_rx_head) {
        return false;
    }
    *out_byte = trk_rx_buf[trk_rx_tail];
    trk_rx_tail = (trk_rx_tail + 1) % TRK_RX_BUF_SIZE;
    return true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        uint8_t byte = trk_rx_buf[trk_rx_head];

        // лог принятых байтов
        UartLogger_Hex("UART_RX:", &byte, 1);

        uint16_t next = (trk_rx_head + 1) % TRK_RX_BUF_SIZE;
        if (next != trk_rx_tail) {
            trk_rx_head = next;
        }

        TrkUart_StartRxIT();
    }
}
