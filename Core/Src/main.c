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

void SystemClock_Config(void);

char display_buffer[40] = "Press a key...";
char eeprom_data[40] = "";

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

void Update_Display(const char* text)
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString((char*)text, Font_7x10, White);
    ssd1306_UpdateScreen();
}

void Update_Display_2Lines(const char* line1, const char* line2)
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString((char*)line1, Font_7x10, White);
    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString((char*)line2, Font_7x10, White);
    ssd1306_UpdateScreen();
}

void log_printf(const char *fmt, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

#define DEBOUNCE_DELAY 50
#define HOLD_CLEAR_DELAY 1000
#define EEPROM_ADDR 0x00

void Scan_Keyboard(void)
{
    static uint32_t last_press_time = 0;
    static uint32_t key_hold_start_time = 0;
    static uint8_t key_is_pressed = 0;

    char current_key = 0;
    uint8_t key_detected = 0;

    for (uint8_t row = 0; row < 5; row++) {
        for (uint8_t r = 0; r < 5; r++)
            HAL_GPIO_WritePin(row_ports[r], row_pins[r], (r == row) ? GPIO_PIN_RESET : GPIO_PIN_SET);

        HAL_Delay(1);

        for (uint8_t col = 0; col < 4; col++) {
            if (HAL_GPIO_ReadPin(col_ports[col], col_pins[col]) == GPIO_PIN_RESET) {
                current_key = keymap[row][col];
                key_detected = 1;
                break;
            }
        }
        if (key_detected) break;
    }

    uint32_t now = HAL_GetTick();

    if (key_detected) {
        if (!key_is_pressed && (now - last_press_time > DEBOUNCE_DELAY)) {
            key_is_pressed = 1;
            last_press_time = now;
            key_hold_start_time = now;

            log_printf("Key detected: %c\r\n", current_key);

            if (current_key == 'C') {
                strcpy(display_buffer, "Press a key...");
                Update_Display(display_buffer);
                log_printf("Action: Clear screen\r\n");

            } else if (current_key == '<') {
                size_t len = strlen(display_buffer);
                if (len > 0 && strcmp(display_buffer, "Press a key...") != 0) {
                    display_buffer[len - 1] = '\0';
                    Update_Display(display_buffer);
                    log_printf("Action: Delete last char\r\n");
                }

            } else if (current_key == 'G') {
                HAL_StatusTypeDef res = EEPROM_Write(EEPROM_ADDR, (uint8_t*)display_buffer, strlen(display_buffer)+1);
                log_printf("Action: Write EEPROM (%d)\r\n", res);
                Update_Display("Write EEPROM");
                HAL_Delay(1000);
                Update_Display(display_buffer);

            } else if (current_key == 'H') {
                memset(eeprom_data, 0, sizeof(eeprom_data));
                HAL_StatusTypeDef res = EEPROM_Read(EEPROM_ADDR, (uint8_t*)eeprom_data, sizeof(eeprom_data));
                log_printf("Action: Read EEPROM (%d)\r\n", res);
                Update_Display_2Lines("Read EEPROM", eeprom_data);
                HAL_Delay(1000);
                Update_Display(display_buffer);

            } else {
                if (strcmp(display_buffer, "Press a key...") == 0)
                    display_buffer[0] = '\0';

                if (strlen(display_buffer) < sizeof(display_buffer) - 1) {
                    size_t len = strlen(display_buffer);
                    display_buffer[len] = current_key;
                    display_buffer[len + 1] = '\0';
                    Update_Display(display_buffer);
                    log_printf("Action: Append char '%c'\r\n", current_key);
                }
            }
        } else if (key_is_pressed && current_key == 'C' && (now - key_hold_start_time > HOLD_CLEAR_DELAY)) {
            strcpy(display_buffer, "Press a key...");
            Update_Display(display_buffer);
            key_hold_start_time = now + 10000;
            log_printf("Action: Hold-clear triggered\r\n");
        }
    } else if (key_is_pressed && (now - last_press_time > DEBOUNCE_DELAY)) {
        key_is_pressed = 0;
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();

    ssd1306_Init();
    Update_Display("Hello!");
    HAL_Delay(2000);
    Update_Display(display_buffer);

    while (1) {
        Scan_Keyboard();
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
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
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

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // Optional
}
#endif
