
/* eeprom.h */
#ifndef EEPROM_H
#define EEPROM_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stddef.h>

#define EEPROM_I2C_ADDR 0x50 << 1

HAL_StatusTypeDef EEPROM_Write(uint16_t mem_address, const uint8_t *data, size_t size);
HAL_StatusTypeDef EEPROM_Read(uint16_t mem_address, uint8_t *data, size_t size);

#endif // EEPROM_H
