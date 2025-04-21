// /Inc/display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/**
 * @brief Инициализация OLED-дисплея.
 *        Настройка I2C и SSD1306 должна быть выполнена до вызова Display_Init().
 */
void Display_Init(void);

/**
 * @brief Очистить экран.
 */
void Display_Clear(void);

/**
 * @brief Показать две строки текста.
 * @param line1 Первая строка (до 20 символов)
 * @param line2 Вторая строка (до 20 символов)
 */
void Display_ShowTwoLines(const char *line1, const char *line2);

/**
 * @brief Показать экран ожидания (Idle).
 */
void Display_ShowIdle(void);

#endif // DISPLAY_H
