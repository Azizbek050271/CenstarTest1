#include "display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "app_timer.h"
#include <string.h>

static bool welcome_active = false;

void Display_Init(void)
{
    ssd1306_Init();
    Display_StartWelcome();
}

void Display_Clear(void)
{
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}

void Display_ShowTwoLines(const char *line1, const char *line2)
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString((char *)line1, Font_7x10, White);
    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString((char *)line2, Font_7x10, White);
    ssd1306_UpdateScreen();
}

void Display_ShowIdle(void)
{
    if (!welcome_active) {
        Display_ShowTwoLines("Idle", "");
    }
}

void Display_StartWelcome(void)
{
    welcome_active = true;
    ssd1306_Fill(Black);
    ssd1306_SetCursor(32, 24);
    ssd1306_WriteString("Censtar", Font_11x18, White);
    ssd1306_UpdateScreen();
    AppTimer_Start(TIMER_WELCOME, 3000, false);
}

void Display_UpdateWelcomeMenu(void)
{
    if (welcome_active && AppTimer_Expired(TIMER_WELCOME)) {
        welcome_active = false;
        ssd1306_Fill(Black);
        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString("Select mode:", Font_7x10, White);
        ssd1306_SetCursor(0, 12);
        ssd1306_WriteString("F - Menu", Font_7x10, White);
        ssd1306_SetCursor(0, 24);
        ssd1306_WriteString("A - View total", Font_7x10, White);
        ssd1306_SetCursor(0, 36);
        ssd1306_WriteString("B - Value mode", Font_7x10, White);
        ssd1306_SetCursor(0, 48);
        ssd1306_WriteString("C - Amount mode", Font_7x10, White);
        ssd1306_SetCursor(0, 60);
        ssd1306_WriteString("D - Full mode", Font_7x10, White);
        ssd1306_UpdateScreen();
    }
}
