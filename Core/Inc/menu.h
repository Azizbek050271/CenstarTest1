// /Inc/menu.h
#ifndef MENU_H
#define MENU_H

#include "eeprom_storage.h"
#include <stdbool.h>

/**
 * @brief Инициализация модуля меню.
 * @param cfg Указатель на текущую конфигурацию (читается/записывается)
 */
void Menu_Init(eeprom_config_t *cfg);

/**
 * @brief Войти в меню настроек.
 * @param cfg Указатель на текущую конфигурацию
 */
void Menu_Enter(eeprom_config_t *cfg);

/**
 * @brief Обработать нажатие клавиши в режиме меню.
 * @param key Символ клавиши
 * @param cfg Указатель на текущую конфигурацию
 */
void Menu_HandleKey(char key, eeprom_config_t *cfg);

/**
 * @brief Выйти из меню.
 * @param cfg    Указатель на текущую конфигурацию
 * @param save   true — сохранить конфиг, false — отменить изменения
 */
void Menu_Exit(eeprom_config_t *cfg, bool save);

/**
 * @brief Проверить, активен ли в данный момент режим меню.
 * @return true, если меню открыто
 */
bool Menu_IsActive(void);

#endif // MENU_H
