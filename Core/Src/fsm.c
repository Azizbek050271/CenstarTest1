// /Src/fsm.c
#include "fsm.h"
#include "uart_logger.h"
#include "display.h"
#include "menu.h"
#include "app_timer.h"
#include "config.h"
#include "eeprom_storage.h"

#define FSM_DEBUG 1

static fsm_state_t state;
static eeprom_config_t cfg;

#if FSM_DEBUG
static const char* FSM_StateName(fsm_state_t s) {
    switch (s) {
    case SM_IDLE: return "IDLE";
    case SM_MENU: return "MENU";
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

    Protocol_GasKitLink.init();
    Menu_Init(&cfg);

    Display_ShowIdle();
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
            LOG_INFO("FSM: Manual menu exit to SM_IDLE\n");
            LOG_FSM_STATE();
        }
        return;
    }

    // TODO: handle keys in other states if needed
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

    // TODO: handle protocol timeouts if needed
}
