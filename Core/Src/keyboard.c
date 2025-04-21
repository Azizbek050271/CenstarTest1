// /Src/keyboard.c
#include "keyboard.h"
#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include <string.h>

// Задержка для дебаунса (мс)
#define DEBOUNCE_DELAY     50U

// Порты и пины строк (PC0..PC4)
static GPIO_TypeDef* row_ports[5] = {
    GPIOC, GPIOC, GPIOC, GPIOC, GPIOC
};
static const uint16_t row_pins[5] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4
};

// Порты и пины столбцов (PB0, PB1, PB11, PB12)
static GPIO_TypeDef* col_ports[4] = {
    GPIOB, GPIOB, GPIOB, GPIOB
};
static const uint16_t col_pins[4] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_11, GPIO_PIN_12
};

// Карта символов клавиш
static const char keymap[5][4] = {
    { 'A', 'F', 'G', 'H' },
    { 'B', '1', '2', '3' },
    { 'C', '4', '5', '6' },
    { 'D', '7', '8', '9' },
    { 'E', '*', '0', 'K' }
};

// Переменные для дебаунса
static uint32_t last_time = 0;
static bool key_pressed = false;

void Keyboard_Init(void)
{
    // Пины уже настроены в MX_GPIO_Init
    last_time = HAL_GetTick();
    key_pressed = false;
}

bool Keyboard_Scan(char *out_key)
{
    char current_key = 0;
    bool detected = false;

    // Сканируем строки и столбцы
    for (uint8_t row = 0; row < 5; row++) {
        // Активируем строку (устанавливаем низкий уровень)
        for (uint8_t r = 0; r < 5; r++) {
            HAL_GPIO_WritePin(row_ports[r], row_pins[r], (r == row) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }
        HAL_Delay(1); // небольшой медиапериод

        // Читаем столбцы
        for (uint8_t col = 0; col < 4; col++) {
            if (HAL_GPIO_ReadPin(col_ports[col], col_pins[col]) == GPIO_PIN_RESET) {
                current_key = keymap[row][col];
                detected = true;
                break;
            }
        }
        if (detected) break;
    }

    uint32_t now = HAL_GetTick();

    if (detected) {
        // Новое нажатие?
        if (!key_pressed && (now - last_time) > DEBOUNCE_DELAY) {
            key_pressed = true;
            last_time = now;
            *out_key = current_key;
            return true;
        }
    } else {
        // Отпуск клавиши?
        if (key_pressed && (now - last_time) > DEBOUNCE_DELAY) {
            key_pressed = false;
            last_time = now;
        }
    }

    return false;
}
