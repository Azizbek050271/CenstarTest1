// /Src/menu.c
#include "menu.h"
#include "display.h"
#include "keyboard.h"
#include "strings.h"
#include "eeprom_storage.h"
#include "app_timer.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"

typedef enum {
    MENU_MAIN,
    MENU_PROTOCOL,
    MENU_UART,
    MENU_PRICE,
    MENU_POSTADDR,
    MENU_NOZZLEADDR,
    MENU_LANGUAGE
} menu_level_t;

static eeprom_config_t *menu_cfg;
static menu_level_t menu_level;
static bool           menu_active;
static uint8_t        sel_index;
static char           input_buf[16];
static uint8_t        input_len;

#define MAIN_ITEMS_COUNT   8
#define PROTO_ITEMS_COUNT  2
#define UART_ITEMS_COUNT   4
#define LANG_ITEMS_COUNT   3

static const uint32_t uart_bauds[UART_ITEMS_COUNT] = {
    9600, 19200, 38400, 115200
};

static void menu_display_current(void)
{
    char buf1[21], buf2[21];
    language_id_t lang = menu_cfg->language_id;

    switch (menu_level) {
    case MENU_MAIN:
        snprintf(buf1, 21, "%s", Str_MenuTitle_Main(lang));
        snprintf(buf2, 21, "%s", Str_MainMenu_Value(lang, sel_index, menu_cfg));
        break;
    case MENU_PROTOCOL:
        snprintf(buf1, 21, "%s", Str_MenuTitle_Protocol(lang));
        snprintf(buf2, 21, "%s", Str_ProtocolMenu_Value(lang, sel_index));
        break;
    case MENU_UART:
        snprintf(buf1, 21, "%s", Str_MenuTitle_UART3(lang));
        snprintf(buf2, 21, "%lu", (unsigned long)uart_bauds[sel_index]);
        break;
    case MENU_LANGUAGE:
        snprintf(buf1, 21, "%s", Str_MenuTitle_Language(lang));
        snprintf(buf2, 21, "%s", Str_LanguageMenu_Value(lang, sel_index));
        break;
    case MENU_PRICE:
        snprintf(buf1, 21, "%s", Str_MenuTitle_Price(lang));
        snprintf(buf2, 21, "%s", input_buf);
        break;
    case MENU_POSTADDR:
        snprintf(buf1, 21, "%s", Str_MenuTitle_PostAddr(lang));
        snprintf(buf2, 21, "%s", input_buf);
        break;
    case MENU_NOZZLEADDR:
        snprintf(buf1, 21, "%s", Str_MenuTitle_NozzleAddr(lang));
        snprintf(buf2, 21, "%s", input_buf);
        break;
    }
    Display_ShowTwoLines(buf1, buf2);
}

static uint32_t parse_price(const char *s)
{
    uint32_t value = 0;
    char *dot = strchr(s, '.');
    if (dot) {
        *dot = '\0';
        uint32_t whole = atoi(s);
        uint32_t frac = atoi(dot+1);
        if (strlen(dot+1) == 1) frac *= 10;
        value = whole*100 + frac;
    } else {
        value = atoi(s)*100;
    }
    return value;
}

void Menu_Init(eeprom_config_t *cfg)
{
    menu_cfg = cfg;
    menu_active = false;
}

void Menu_Enter(eeprom_config_t *cfg)
{
    (void)cfg;
    menu_level = MENU_MAIN;
    sel_index = 0;
    menu_active = true;
    menu_display_current();
}

void Menu_HandleKey(char key, eeprom_config_t *cfg)
{
    if (!menu_active) return;

    switch (menu_level) {
    case MENU_MAIN:
        if (key == KEY_LEFT) {
            sel_index = (sel_index + MAIN_ITEMS_COUNT - 1) % MAIN_ITEMS_COUNT;
            menu_display_current();
        } else if (key == KEY_RIGHT) {
            sel_index = (sel_index + 1) % MAIN_ITEMS_COUNT;
            menu_display_current();
        } else if (key == KEY_CONFIRM) {
            switch (sel_index) {
            case 0: menu_level = MENU_PROTOCOL; sel_index = cfg->protocol - 1; break;
            case 1: menu_level = MENU_UART; for (uint8_t i = 0; i < UART_ITEMS_COUNT; i++) if (uart_bauds[i] == cfg->price_cent) { sel_index = i; break; } break;
            case 2: menu_level = MENU_PRICE; snprintf(input_buf, sizeof(input_buf), "%u.%02u", (unsigned)(cfg->price_cent / 100), (unsigned)(cfg->price_cent % 100)); input_len = strlen(input_buf); break;
            case 3: menu_level = MENU_POSTADDR; snprintf(input_buf, sizeof(input_buf), "%02u", (unsigned)cfg->protocol); input_len = strlen(input_buf); break;
            case 4: menu_level = MENU_NOZZLEADDR; snprintf(input_buf, sizeof(input_buf), "%02u", (unsigned)cfg->post_addr); input_len = strlen(input_buf); break;
            case 5: menu_level = MENU_LANGUAGE; sel_index = cfg->language_id - 1; break;
            case 6: Menu_Exit(cfg, true); return;
            case 7: Menu_Exit(cfg, false); return;
            }
            menu_display_current();
        }
        break;

    case MENU_PROTOCOL:
        if (key == KEY_LEFT) {
            sel_index = (sel_index + PROTO_ITEMS_COUNT - 1) % PROTO_ITEMS_COUNT;
        } else if (key == KEY_RIGHT) {
            sel_index = (sel_index + 1) % PROTO_ITEMS_COUNT;
        } else if (key == KEY_CONFIRM) {
            cfg->protocol = (proto_id_t)(sel_index + 1);
            menu_level = MENU_MAIN;
            sel_index = 0;
        } else if (key == KEY_CANCEL) {
            menu_level = MENU_MAIN;
            sel_index = 0;
        }
        menu_display_current();
        break;

    case MENU_UART:
        if (key == KEY_LEFT) {
            sel_index = (sel_index + UART_ITEMS_COUNT - 1) % UART_ITEMS_COUNT;
        } else if (key == KEY_RIGHT) {
            sel_index = (sel_index + 1) % UART_ITEMS_COUNT;
        } else if (key == KEY_CONFIRM) {
            cfg->post_addr = uart_bauds[sel_index];
            menu_level = MENU_MAIN;
            sel_index = 1;
        } else if (key == KEY_CANCEL) {
            menu_level = MENU_MAIN;
            sel_index = 1;
        }
        menu_display_current();
        break;

    case MENU_LANGUAGE:
        if (key == KEY_LEFT) {
            sel_index = (sel_index + LANG_ITEMS_COUNT - 1) % LANG_ITEMS_COUNT;
        } else if (key == KEY_RIGHT) {
            sel_index = (sel_index + 1) % LANG_ITEMS_COUNT;
        } else if (key == KEY_CONFIRM) {
            cfg->language_id = (language_id_t)(sel_index + 1);
            menu_level = MENU_MAIN;
            sel_index = 5;
        } else if (key == KEY_CANCEL) {
            menu_level = MENU_MAIN;
            sel_index = 5;
        }
        menu_display_current();
        break;

    case MENU_PRICE:
    case MENU_POSTADDR:
    case MENU_NOZZLEADDR:
        if ((key >= '0' && key <= '9') || (menu_level == MENU_PRICE && key == KEY_POINT)) {
            if (input_len < sizeof(input_buf) - 1) {
                input_buf[input_len++] = (key == KEY_POINT) ? '.' : key;
                input_buf[input_len] = '\0';
                menu_display_current();
            }
        } else if (key == KEY_CONFIRM) {
            if (menu_level == MENU_PRICE) {
                cfg->price_cent = parse_price(input_buf);
                sel_index = 2;
            } else if (menu_level == MENU_POSTADDR) {
                cfg->post_addr = (uint8_t)atoi(input_buf);
                sel_index = 3;
            } else {
                cfg->nozzle_addr = (uint8_t)atoi(input_buf);
                sel_index = 4;
            }
            menu_level = MENU_MAIN;
            menu_display_current();
        } else if (key == KEY_CANCEL) {
            menu_level = MENU_MAIN;
            menu_display_current();
        }
        break;
    }
}

void Menu_Exit(eeprom_config_t *cfg, bool save)
{
    if (save) {
        EepromStorage_Save(cfg);
        Display_ShowTwoLines("Saved", "");
        HAL_Delay(1000);
    }
    menu_active = false;
    Display_Clear();
}

bool Menu_IsActive(void)
{
    return menu_active;
}
