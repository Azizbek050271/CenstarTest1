// /Inc/app_timer.h
#ifndef APP_TIMER_H
#define APP_TIMER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Запустить таймер.
 * @param id      Идентификатор таймера (0..APP_TIMER_COUNT-1)
 * @param timeout_ms Время до срабатывания в миллисекундах
 * @param reload  true — автоперезапуск по наступлению, false — одноразовый
 */
void AppTimer_Start(uint8_t id, uint32_t timeout_ms, bool reload);

/**
 * @brief Остановить таймер.
 * @param id Идентификатор таймера
 */
void AppTimer_Stop(uint8_t id);

/**
 * @brief Проверить, истёк ли таймер.
 * @param id Идентификатор таймера
 * @return true, если таймер сработал (и остановлен, если был одноразовым)
 */
bool AppTimer_Expired(uint8_t id);

#endif // APP_TIMER_H
