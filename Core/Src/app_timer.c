#include "app_timer.h"
#include "config.h"
#include "stm32f4xx_hal.h"

typedef struct {
    uint32_t    start;
    uint32_t    duration;
    bool        running;
    bool        reload;
} timer_t;

static timer_t timers[APP_TIMER_COUNT];

void AppTimer_Start(TimerId id, uint32_t timeout_ms, bool reload)
{
    if (id >= APP_TIMER_COUNT) return;
    timers[id].start    = HAL_GetTick();
    timers[id].duration = timeout_ms;
    timers[id].reload   = reload;
    timers[id].running  = true;
}

void AppTimer_Stop(TimerId id)
{
    if (id >= APP_TIMER_COUNT) return;
    timers[id].running = false;
}

bool AppTimer_Expired(TimerId id)
{
    if (id >= APP_TIMER_COUNT) return false;
    timer_t *t = &timers[id];
    if (!t->running) return false;
    uint32_t now = HAL_GetTick();
    if ((uint32_t)(now - t->start) >= t->duration) {
        if (t->reload) {
            t->start = now;
        } else {
            t->running = false;
        }
        return true;
    }
    return false;
}
