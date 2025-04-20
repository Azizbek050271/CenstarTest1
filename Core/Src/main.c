/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Основная программа
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * Все права защищены.
  *
  * Это программное обеспечение лицензировано на условиях, указанных в файле LICENSE
  * в корневой директории данного программного компонента.
  * Если файл LICENSE отсутствует, ПО предоставляется "КАК ЕСТЬ".
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "eeprom.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

GPIO_TypeDef* row_ports[5] = { GPIOC, GPIOC, GPIOC, GPIOC, GPIOC };
uint16_t      row_pins[5]  = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4 };
GPIO_TypeDef* col_ports[4] = { GPIOB, GPIOB, GPIOB, GPIOB };
uint16_t      col_pins[4]  = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_11, GPIO_PIN_12 };

char keymap[5][4] = {
    { 'A', 'F', 'G', 'H' },
    { 'B', '1', '2', '3' },
    { 'C', '4', '5', '6' },
    { 'D', '7', '8', '9' },
    { 'E', '*', '0', '<' }
};

char display_buffer[40] = "Press a key...";

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();
    // MX_USART3_UART_Init(); // Временно закомментировано для отладки

    ssd1306_Init();
    Update_Display("Hello!");
    HAL_Delay(2000);
    Update_Display(display_buffer);

    while (1)
    {
        HAL_Delay(50);
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // Optional: printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif