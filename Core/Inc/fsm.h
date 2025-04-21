// /Inc/fsm.h
#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include "eeprom_storage.h"
#include "protocol_gaskit.h"

/** Состояния конечного автомата */
typedef enum {
    SM_IDLE,         ///< Ожидание, пульт готов
    SM_TRANSACTION,  ///< Ввод параметров транзакции
    SM_AUTHORIZED,   ///< Ожидание поднятия пистолета
    SM_FUELLING,     ///< Идёт заправка
    SM_STOP,         ///< Остановка/финализация
    SM_END,          ///< Конец транзакции
    SM_MENU          ///< Режим меню настроек
} fsm_state_t;

/**
 * Инициализация FSM:
 *  - загрузка конфигурации из EEPROM
 *  - инициализация протокола
 *  - инициализация меню
 *  - вывод начального экрана
 */
void FSM_Init(void);

/**
 * Обработка события нажатия клавиши
 * @param key символ клавиши
 */
void FSM_EventKey(char key);

/**
 * Обработка принятого кадра от ТРК
 * @param resp распарсенный кадр протокола GasKitLink
 */
void FSM_EventProtocol(const gaskit_parsed_t *resp);

/**
 * Тикер FSM: проверка таймаутов меню и протокола
 * Вызывать в основном цикле раз в 1–10 мс
 */
void FSM_Tick(void);

#endif // FSM_H
