// /Src/app_timer.c
#include "app_timer.h"
#include "config.h"        // для APP_TIMER_COUNT
#include "stm32f4xx_hal.h" // для HAL_GetTick()

typedef struct {
    uint32_t    start;
    uint32_t    duration;
    bool        running;
    bool        reload;
} timer_t;

static timer_t timers[APP_TIMER_COUNT];

void AppTimer_Start(uint8_t id, uint32_t timeout_ms, bool reload)
{
    if (id >= APP_TIMER_COUNT) return;
    timers[id].start    = HAL_GetTick();
    timers[id].duration = timeout_ms;
    timers[id].reload   = reload;
    timers[id].running  = true;
}

void AppTimer_Stop(uint8_t id)
{
    if (id >= APP_TIMER_COUNT) return;
    timers[id].running = false;
}

bool AppTimer_Expired(uint8_t id)
{
    if (id >= APP_TIMER_COUNT) return false;
    timer_t *t = &timers[id];
    if (!t->running) return false;
    uint32_t now = HAL_GetTick();
    // проверка с учётом возможного переполнения HAL_GetTick()
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
