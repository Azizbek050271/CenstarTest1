// /Src/fsm.c
#include "fsm.h"
#include "uart_logger.h"
#include "display.h"
#include "menu.h"
#include "app_timer.h"
#include "config.h"
#include "eeprom_storage.h"

// Текущее состояние FSM
static fsm_state_t state;

// Текущая конфигурация, загруженная из EEPROM
static eeprom_config_t cfg;

void FSM_Init(void)
{
    // Инициализация EEPROM-хранилища
    if (!EepromStorage_Init()) {
        LOG_ERROR("FSM_Init: EEPROM init failed");
    }
    EepromStorage_Load(&cfg);

    // Инициализация протокола обмена с ТРК
    Protocol_GasKitLink.init();

    // Инициализация модуля меню
    Menu_Init(&cfg);

    // Вывод экрана ожидания
    Display_ShowIdle();

    state = SM_IDLE;
    LOG_INFO("FSM initialized. State=SM_IDLE");
}

void FSM_EventKey(char key)
{
    LOG_INFO("FSM_EventKey: %c", key);

    // Клавиша входа в меню
    if (key == 'F') {
        if (state != SM_MENU) {
            Menu_Enter(&cfg);
            AppTimer_Start(TIMER_MENU_TIMEOUT, MENU_TIMEOUT_MS, false);
            state = SM_MENU;
            LOG_INFO("Entered SM_MENU");
        }
        return;
    }

    // Если в меню — передаём клавишу в модуль меню
    if (state == SM_MENU) {
        Menu_HandleKey(key, &cfg);
        return;
    }

    // TODO: обработка клавиш в рабочих состояниях (SM_IDLE, SM_TRANSACTION и т.д.)
}

void FSM_EventProtocol(const gaskit_parsed_t *resp)
{
    LOG_INFO("FSM_EventProtocol: addr=%u cmd=%c len=%u",
             resp->address, resp->cmd, (unsigned)resp->data_len);

    // TODO: разбираем resp->cmd и resp->data в зависимости от state
    // и переводим FSM в следующие состояния, обновляем дисплей и т.д.
}

void FSM_Tick(void)
{
    // Авто-выход из меню по таймауту
    if (state == SM_MENU && AppTimer_Expired(TIMER_MENU_TIMEOUT)) {
        Menu_Exit(&cfg, false);
        state = SM_IDLE;
        Display_ShowIdle();
        LOG_INFO("Menu timeout, back to SM_IDLE");
    }

    // TODO: проверка таймаутов протокола:
    // if (AppTimer_Expired(TIMER_PROTO_RESPONSE)) { ... }
    // if (AppTimer_Expired(TIMER_PROTO_INTERFRAME)) { ... }
}
