#ifndef APP_TIMER_H
#define APP_TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"  // для TimerId

/**
 * @brief Запустить таймер.
 * @param id         Идентификатор таймера (enum TimerId)
 * @param timeout_ms Время до срабатывания в миллисекундах
 * @param reload     true — автоперезапуск по наступлению, false — одноразовый
 */
void AppTimer_Start(TimerId id, uint32_t timeout_ms, bool reload);

/**
 * @brief Остановить таймер.
 * @param id Идентификатор таймера
 */
void AppTimer_Stop(TimerId id);

/**
 * @brief Проверить, истёк ли таймер.
 * @param id Идентификатор таймера
 * @return true, если таймер сработал (и остановлен, если был одноразовым)
 */
bool AppTimer_Expired(TimerId id);

#endif // APP_TIMER_H
