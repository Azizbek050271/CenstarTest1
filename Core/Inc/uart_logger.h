// /Inc/uart_logger.h
#ifndef UART_LOGGER_H
#define UART_LOGGER_H

#include <stdint.h>

/**
 * @brief Инициализация логгера.
 *        Настройка UART2 должна быть выполнена до вызова Logger_Init().
 */
void Logger_Init(void);

/**
 * @brief Запись лог-сообщения.
 * @param level Уровень (например, "I" или "E")
 * @param fmt   Формат printf‑строки
 * @param ...   Аргументы
 */
void Logger_Log(const char *level, const char *fmt, ...);

/**
 * @brief Лог информационных сообщений.
 */
#define LOG_INFO(fmt, ...)  Logger_Log("I", fmt, ##__VA_ARGS__)

/**
 * @brief Лог ошибок.
 */
#define LOG_ERROR(fmt, ...) Logger_Log("E", fmt, ##__VA_ARGS__)

#endif // UART_LOGGER_H
