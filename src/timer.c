#include "peripherals/base.h"
#include "peripherals/timer.h"

#include "assertions.h"

typedef struct
{
    uint32_t cs;
    uint32_t clo;
    uint32_t chi;
    uint32_t c0;
    uint32_t c1;
    uint32_t c2;
    uint32_t c3;
} system_timer_registers_t;

ASSERT_SIZE(system_timer_registers_t, 28);

#define SYSTEM_TIMER ((volatile system_timer_registers_t*)(PERIPHERAL_BASE + 0x00003000))

uint64_t timer_get_ticks(void)
{
    uint64_t h, l;

    do
    {
        h = SYSTEM_TIMER->chi;
        l = SYSTEM_TIMER->clo;
    } while (h != SYSTEM_TIMER->chi);

    return (h << 32) | l;
}

void timer_wait(uint64_t us)
{
    us += timer_get_ticks();
    while (timer_get_ticks() < us) {}
}
