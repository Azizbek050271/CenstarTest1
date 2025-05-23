#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void USART3_SetBaudrate(uint32_t baudrate);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
