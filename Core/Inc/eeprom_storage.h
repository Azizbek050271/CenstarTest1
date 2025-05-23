// /Inc/eeprom_storage.h
#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <stdint.h>
#include <stdbool.h>

/** Версия структуры в EEPROM, для миграции */
#define EEPROM_CONFIG_VERSION  0x01

/** Идентификаторы протоколов */
typedef uint8_t proto_id_t;
#define PROTO_ID_GASKITLINK    1
#define PROTO_ID_AZT2          2

/** Идентификаторы языков интерфейса */
typedef uint8_t language_id_t;
#define LANG_EN                1
#define LANG_RU                2
#define LANG_UZ                3

/** Структура конфигурации, сохраняемая в EEPROM */
typedef struct {
    uint8_t       version;        /**< версия структуры (EEPROM_CONFIG_VERSION) */
    uint32_t      price_cent;     /**< цена за литр в центах/теньге */
    proto_id_t    protocol;       /**< выбранный протокол */
    uint8_t       post_addr;      /**< адрес поста */
    uint8_t       nozzle_addr;    /**< адрес рукава */
    uint32_t      uart3_baud;     /**< скорость UART3 */
    uint8_t       reserved[4];    /**< резерв на будущее */
    language_id_t language_id;    /**< выбранный язык интерфейса */
    uint8_t       reserved2[1];   /**< выравнивание */
    uint16_t      crc;            /**< CRC16 от первых (size-2) байт */
} eeprom_config_t;

bool EepromStorage_Init(void);
bool EepromStorage_Load(eeprom_config_t *cfg);
bool EepromStorage_Save(const eeprom_config_t *cfg);
void EepromStorage_ResetDefaults(eeprom_config_t *cfg);

#endif // EEPROM_STORAGE_H
