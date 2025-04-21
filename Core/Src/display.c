// /Src/display.c
#include "display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <string.h>

/**
 * @brief Инициализация дисплея SSD1306.
 */
void Display_Init(void)
{
    ssd1306_Init();
    Display_Clear();
}

/**
 * @brief Полная очистка экрана.
 */
void Display_Clear(void)
{
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}

/**
 * @brief Отобразить две строки на экране.
 */
void Display_ShowTwoLines(const char *line1, const char *line2)
{
    // Очистить буфер
    ssd1306_Fill(Black);

    // Первая строка
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString((char*)line1, Font_7x10, White);

    // Вторая строка
    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString((char*)line2, Font_7x10, White);

    // Обновить экран
    ssd1306_UpdateScreen();
}

/**
 * @brief Показать экран ожидания (Idle).
 */
void Display_ShowIdle(void)
{
    Display_ShowTwoLines("Idle", "");
}
