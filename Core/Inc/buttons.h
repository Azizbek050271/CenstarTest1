// /Core/Inc/buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

typedef enum {
    BTN_NONE = 0,
    BTN_F,
    BTN_A,
    BTN_B,
    BTN_C,
    BTN_D,
    BTN_E,
    BTN_K,
} Button_t;

Button_t Buttons_Get(void);
int Buttons_IsPressed(Button_t btn);

#endif // BUTTONS_H
