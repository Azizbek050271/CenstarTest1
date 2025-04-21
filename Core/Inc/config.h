#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include "eeprom_storage.h"  // для определения proto_id_t и language_id_t

// Адрес в EEPROM, с которого хранится структура конфигурации
#define EEPROM_CONFIG_ADDR      0x0000U

// Значения по умолчанию для конфигурации
#define DEFAULT_PROTOCOL_ID     PROTO_ID_GASKITLINK
#define DEFAULT_UART3_BAUD      9600U
#define DEFAULT_PRICE_CENT      0U
#define DEFAULT_POST_ADDR       0x00U
#define DEFAULT_NOZZLE_ADDR     0x01U
#define DEFAULT_LANGUAGE_ID     LANG_EN

// Размер кольцевого буфера для приёма по UART3 (TRK)
#define TRK_RX_BUF_SIZE         256U

// Таймауты протокола GasKitLink (в миллисекундах)
#define PROTO_TIMEOUT_MS            50U
#define PROTO_INTERFRAME_TIMEOUT_MS  3U

// Таймаут меню (авто‑выход из меню) (ms)
#define MENU_TIMEOUT_MS          30000U

// Перечисление таймеров
typedef enum {
    TIMER_PROTO_RESPONSE,
    TIMER_PROTO_INTERFRAME,
    TIMER_MENU_TIMEOUT,
    TIMER_MENU_MESSAGE,
	 TIMER_WELCOME,  // добавить сюда
    TIMER_COUNT
} TimerId;

#define APP_TIMER_COUNT TIMER_COUNT

// Раскладка клавиш меню
#define KEY_LEFT     'G'
#define KEY_RIGHT    'H'
#define KEY_ACCEPT   'B'
#define KEY_DECLINE  'C'
#define KEY_CONFIRM  'K'
#define KEY_CANCEL   'E'
#define KEY_POINT    '*'

#endif // CONFIG_H
