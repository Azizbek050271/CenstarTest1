// /Src/fsm.c
#include "fsm.h"
#include "uart_logger.h"
#include "display.h"
#include "menu.h"
#include "app_timer.h"
#include "config.h"
#include "eeprom_storage.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define FSM_DEBUG 1
#define START_MENU_TIMEOUT_MS 3000

static fsm_state_t state;
static eeprom_config_t cfg;
static const TimerId IDLE_TIMER = TIMER_WELCOME;

#if FSM_DEBUG
static const char* FSM_StateName(fsm_state_t s) {
    switch (s) {
    case SM_IDLE: return "IDLE";
    case SM_MENU: return "MENU";
    case SM_MODE_SELECT: return "MODE_SELECT";
    case SM_VALUE_INPUT: return "VALUE_INPUT";
    case SM_AMOUNT_INPUT: return "AMOUNT_INPUT";
    case SM_FULL_MODE: return "FULL_MODE";
    default: return "UNKNOWN";
    }
}
#define LOG_FSM_STATE() LOG_INFO("FSM State = %s\n", FSM_StateName(state))
#else
#define LOG_FSM_STATE() do {} while(0)
#endif

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

void FSM_Init(void)
{
    if (!EepromStorage_Init()) {
        LOG_ERROR("FSM_Init: EEPROM init failed\n");
    }
    EepromStorage_Load(&cfg);

    Protocol_GasKitLink.init();
    Menu_Init(&cfg);

    Display_ShowIdle();
    AppTimer_Start(IDLE_TIMER, START_MENU_TIMEOUT_MS, false);
    state = SM_IDLE;
    LOG_INFO("FSM initialized. State=SM_IDLE\n");
    LOG_FSM_STATE();
}

void FSM_EventKey(char key)
{
    LOG_INFO("FSM_EventKey: %c\n", key);

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
            Display_ShowTwoLines("Total", "(not impl)");
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
        LOG_FSM_STATE();
        return;
    }
}

void FSM_EventProtocol(const gaskit_parsed_t *resp)
{
    LOG_INFO("FSM_EventProtocol: addr=%u cmd=%c len=%u\n",
             resp->address, resp->cmd, (unsigned)resp->data_len);

    // TODO: handle protocol events depending on FSM state
}

void FSM_Tick(void)
{
    if (state == SM_MENU) {
        Menu_Tick();
    }

    if (state == SM_IDLE && AppTimer_Expired(IDLE_TIMER)) {
        FSM_ShowStartMenu();
    }

    // TODO: handle protocol timeouts if needed
}
