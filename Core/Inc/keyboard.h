#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

/**
 * @brief  Инициализация модуля клавиатуры.
 *         Конфигурация GPIO должна быть выполнена ранее (MX_GPIO_Init).
 */
void Keyboard_Init(void);

/**
 * @brief  Сканирование клавиатуры 5×4.
 * @param  out_key Указатель для возврата найденного символа клавиши.
 * @return true, если обнаружено новое нажатие (после дебаунса), false иначе.
 */
bool Keyboard_Scan(char *out_key);

#endif // KEYBOARD_H
