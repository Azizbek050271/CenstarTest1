#include "eeprom_storage.h"
#include "eeprom.h"
#include "crc.h"
#include "config.h"    // для EEPROM_CONFIG_ADDR, DEFAULT_*
#include <string.h>

#define CONFIG_ADDR  EEPROM_CONFIG_ADDR
#define CONFIG_SIZE  sizeof(eeprom_config_t)

/** Вспомогательная: вычислить CRC16 от первых (CONFIG_SIZE - 2) байт */
uint16_t CalcCrc(const eeprom_config_t *cfg)
{
    return CRC16_Calc((const uint8_t*)cfg, CONFIG_SIZE - sizeof(cfg->crc));
}

void EepromStorage_ResetDefaults(eeprom_config_t *cfg)
{
    memset(cfg, 0, CONFIG_SIZE);
    cfg->version     = EEPROM_CONFIG_VERSION;
    cfg->price_cent  = DEFAULT_PRICE_CENT;
    cfg->protocol    = DEFAULT_PROTOCOL_ID;
    cfg->post_addr   = DEFAULT_POST_ADDR;
    cfg->nozzle_addr = DEFAULT_NOZZLE_ADDR;
    cfg->language_id = DEFAULT_LANGUAGE_ID;
    cfg->crc = CalcCrc(cfg);
}

bool EepromStorage_Load(eeprom_config_t *cfg)
{
    if (EEPROM_Read(CONFIG_ADDR, (uint8_t*)cfg, CONFIG_SIZE) != HAL_OK) {
        return false;
    }
    if (cfg->version != EEPROM_CONFIG_VERSION) {
        return false;
    }
    uint16_t expected = CalcCrc(cfg);
    if (cfg->crc != expected) {
        return false;
    }
    return true;
}

bool EepromStorage_Save(const eeprom_config_t *cfg)
{
    eeprom_config_t tmp = *cfg;
    tmp.crc = CalcCrc(&tmp);
    if (EEPROM_Write(CONFIG_ADDR, (uint8_t*)&tmp, CONFIG_SIZE) != HAL_OK) {
        return false;
    }
    return true;
}

bool EepromStorage_Init(void)
{
    eeprom_config_t cfg;
    if (!EepromStorage_Load(&cfg)) {
        EepromStorage_ResetDefaults(&cfg);
        return EepromStorage_Save(&cfg);
    }
    return true;
}
