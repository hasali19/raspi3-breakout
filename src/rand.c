#include "peripherals/rand.h"
#include "peripherals/base.h"

#include "assertions.h"

typedef struct
{
    uint32_t ctrl;
    uint32_t status;
    uint32_t data;
    uint32_t res;
    uint32_t interrupt_mask;
} rng_registers_t;

ASSERT_SIZE(rng_registers_t, 20);

#define RNG ((volatile rng_registers_t*)(PERIPHERAL_BASE + 0x00104000))

void rand_init(void)
{
    RNG->ctrl = 0x40000;

    // Mask interrupt
    RNG->interrupt_mask |= 1;

    // Enable rng
    RNG->ctrl |= 1;

    // Wait to gain entropy
    while (!((RNG->status) >> 24));
}

uint32_t rand_generate(uint32_t min, uint32_t max)
{
    return RNG->data % (max - min) + min;
}
