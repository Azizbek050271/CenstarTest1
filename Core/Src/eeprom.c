
#include "eeprom.h"
#include "main.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef EEPROM_Write(uint16_t mem_address, const uint8_t *data, size_t size)
{
    uint8_t buffer[2 + size];
    buffer[0] = (mem_address >> 8) & 0xFF;
    buffer[1] = mem_address & 0xFF;
    memcpy(&buffer[2], data, size);

    HAL_StatusTypeDef res = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_I2C_ADDR, buffer, size + 2, HAL_MAX_DELAY);
    HAL_Delay(5); // EEPROM write delay
    return res;
}

HAL_StatusTypeDef EEPROM_Read(uint16_t mem_address, uint8_t *data, size_t size)
{
    uint8_t addr[2];
    addr[0] = (mem_address >> 8) & 0xFF;
    addr[1] = mem_address & 0xFF;

    HAL_StatusTypeDef res = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_I2C_ADDR, addr, 2, HAL_MAX_DELAY);
    if (res != HAL_OK) return res;

    return HAL_I2C_Master_Receive(&hi2c1, EEPROM_I2C_ADDR, data, size, HAL_MAX_DELAY);
}
