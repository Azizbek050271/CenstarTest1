// /Src/main.c
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "config.h"
#include "uart_logger.h"
#include "display.h"
#include "keyboard.h"
#include "app_timer.h"
#include "fsm.h"
#include "protocol_gaskit.h"

void SystemClock_Config(void);

int main(void)
{
    // --------------------
    // Базовая инициализация
    // --------------------
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();

    // --------------------
    // Инициализация модулей
    // --------------------
    Logger_Init();      // логирование по USART2
    Display_Init();     // OLED-дисплей
    Keyboard_Init();    // клавиатура 5×4

    FSM_Init();         // FSM + EEPROM + Protocol init

    // --------------------
    // Главный цикл
    // --------------------
    while (1) {
        char key;
        // 1) Обработка нажатий клавиш
        if (Keyboard_Scan(&key)) {
            FSM_EventKey(key);
        }

        // 2) Попытка принять кадр от ТРК (UART3)
        uint8_t frame[GASKIT_MAX_FRAME_SIZE];
        size_t  len;
        if (Protocol_GasKitLink.recv_resp(frame, &len, PROTO_INTERFRAME_TIMEOUT_MS) == PROTO_OK) {
            gaskit_parsed_t parsed;
            if (Protocol_GasKitLink.parse(frame, len, &parsed) == PROTO_OK) {
                FSM_EventProtocol(&parsed);
            }
        }

        // 3) Тикер FSM (таймауты меню и протокола)
        FSM_Tick();
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 8;
    RCC_OscInitStruct.PLL.PLLN       = 336;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK   |
                                       RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1  |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // Пользователь может добавить вывод в лог:
    // LOG_ERROR("ASSERT failed: %s:%lu", file, (unsigned long)line);
}
#endif
