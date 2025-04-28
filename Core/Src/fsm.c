#include "fsm.h"
#include "uart_logger.h"
#include "display.h"
#include "menu.h"
#include "app_timer.h"
#include "config.h"
#include "eeprom_storage.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "stm32f4xx_hal.h"
#include "protocol_gaskit.h"
#include "uart_trk.h"
#include "commands_gaskit.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FSM_DEBUG 1
#define START_MENU_TIMEOUT_MS 3000

static fsm_state_t state;
static fsm_state_t prev_state;
static eeprom_config_t cfg;
static const TimerId IDLE_TIMER = TIMER_WELCOME;
static uint8_t e_press_count = 0;

static char input_buf[16] = {0};
static uint8_t input_len = 0;
static float input_value = 0.0f;
static bool waiting_total = false;

#if FSM_DEBUG
static const char* FSM_StateName(fsm_state_t s) {
    switch (s) {
    case SM_IDLE: return "IDLE";
    case SM_MENU: return "MENU";
    case SM_MODE_SELECT: return "MODE_SELECT";
    case SM_VALUE_INPUT: return "VALUE_INPUT";
    case SM_AMOUNT_INPUT: return "AMOUNT_INPUT";
    case SM_FULL_MODE: return "FULL_MODE";
    case SM_AUTHORIZED: return "AUTHORIZED";
    default: return "UNKNOWN";
    }
}
#define LOG_FSM_STATE() LOG_INFO("FSM State = %s\n", FSM_StateName(state))
#else
#define LOG_FSM_STATE() do {} while(0)
#endif

void FSM_Init(void)
{
    if (!EepromStorage_Init()) {
        LOG_ERROR("FSM_Init: EEPROM init failed\n");
    }
    EepromStorage_Load(&cfg);

    USART3_SetBaudrate(cfg.uart3_baud);
    LOG_INFO("Set UART3 to %lu baud", cfg.uart3_baud);

    Protocol_GasKitLink.init();
    Menu_Init(&cfg);

    Display_Init();
    Display_ShowIdle();
    AppTimer_Start(IDLE_TIMER, START_MENU_TIMEOUT_MS, false);
    state = SM_IDLE;
    LOG_INFO("FSM initialized. State=SM_IDLE\n");
    LOG_FSM_STATE();
}

void FSM_SendC0Request(void)
{
    uint8_t frame[16];
    uint8_t len = Gaskit_ReqTotals(frame, cfg.post_addr, cfg.nozzle_addr);
    UartLogger_Hex("UART_TX:", frame, len);
    TrkUart_Send(frame, len);
    waiting_total = true;
    Display_ShowTwoLines("TOTAL:", "WAITING...");
    LOG_INFO("FSM: Sent C0 request\n");
}

static void FSM_ShowStartMenu(void)
{
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

    state = SM_MODE_SELECT;
    LOG_INFO("FSM: Entered SM_MODE_SELECT\n");
    LOG_FSM_STATE();
}

void FSM_EventKey(char key)
{
    LOG_INFO("FSM_EventKey: %c\n", key);

    if (waiting_total && key == 'E') {
        waiting_total = false;
        state = SM_IDLE;
        Display_ShowIdle();
        LOG_INFO("FSM: Exit total view to SM_IDLE\n");
        LOG_FSM_STATE();
        return;
    }

    if (state == SM_IDLE) {
        if (key == 'E') {
            FSM_ShowStartMenu();
        }
        return;
    }

    if (key == 'F') {
        if (state != SM_MENU) {
            Menu_Enter(&cfg);
            state = SM_MENU;
            LOG_INFO("Entered SM_MENU\n");
            LOG_FSM_STATE();
        }
        return;
    }

    if (state == SM_MENU) {
        Menu_HandleKey(key, &cfg);

        if (!Menu_IsActive()) {
            state = SM_IDLE;
            Display_ShowIdle();
            AppTimer_Start(IDLE_TIMER, START_MENU_TIMEOUT_MS, false);
            LOG_INFO("FSM: Manual menu exit to SM_IDLE\n");
            LOG_FSM_STATE();
        }
        return;
    }

    if (state == SM_MODE_SELECT) {
        switch (key) {
        case 'A':
            FSM_SendC0Request();
            break;
        case 'B':
            state = SM_VALUE_INPUT;
            Display_ShowTwoLines("Enter", "volume");
            break;
        case 'C':
            state = SM_AMOUNT_INPUT;
            Display_ShowTwoLines("Enter", "amount");
            break;
        case 'D':
            state = SM_FULL_MODE;
            Display_ShowTwoLines("Full mode", "selected");
            break;
        }
        e_press_count = 0;
        LOG_FSM_STATE();
        return;
    }
}

void FSM_EventProtocol(const gaskit_parsed_t *resp)
{
    LOG_INFO("FSM_EventProtocol: addr=%u cmd=%c len=%u\n",
             resp->address, resp->cmd, (unsigned)resp->data_len);

    // отобразить сырые данные вне зависимости от парсинга
    char raw[17] = {0};
    memcpy(raw, resp->data, (resp->data_len > 16 ? 16 : resp->data_len));
    Display_ShowTwoLines("RAW RX:", raw);
    LOG_INFO("FSM RAW = %s\n", raw);

    // оригинальная обработка
    if (waiting_total && resp->cmd == 'C' && resp->data_len > 2 && resp->data[0] == '1' && resp->data[1] == ';') {
        waiting_total = false;
        char data_copy[16] = {0};
        strncpy(data_copy, (const char *)&resp->data[2], sizeof(data_copy) - 1);
        if (strlen(data_copy) >= 3) {
            size_t len = strlen(data_copy);
            char formatted[20];
            snprintf(formatted, sizeof(formatted), "%.*s.%s", (int)(len - 3), data_copy, data_copy + len - 3);
            Display_ShowTwoLines("TOTAL:", formatted);
            LOG_INFO("FSM: TOTAL = %s\n", formatted);
        }
    }
}

void FSM_Tick(void)
{
    if (state == SM_MENU) {
        Menu_Tick();
    }

    if (state == SM_IDLE && AppTimer_Expired(IDLE_TIMER)) {
        FSM_ShowStartMenu();
    }
}
