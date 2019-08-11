#include "peripherals/base.h"
#include "peripherals/gpio.h"

#include "assertions.h"
#include "utils.h"

typedef struct
{
    uint32_t fsel[6];
    uint32_t res1;
    uint32_t set[2];
    uint32_t res2;
    uint32_t clr[2];
    uint32_t res3;
    uint32_t lev[2];
    uint32_t res4;
    uint32_t eds[2];
    uint32_t res5;
    uint32_t ren[2];
    uint32_t res6;
    uint32_t fen[2];
    uint32_t res7;
    uint32_t hen[2];
    uint32_t res8;
    uint32_t len[2];
    uint32_t res9;
    uint32_t aren[2];
    uint32_t res10;
    uint32_t afen[2];
    uint32_t res11;
    uint32_t pud;
    uint32_t pudclk[2];
} gpio_registers_t;

ASSERT_SIZE(gpio_registers_t, 160);

#define GPIO ((volatile gpio_registers_t*)(PERIPHERAL_BASE + 0x00200000))

void gpio_setup(uint32_t pin, uint32_t mode)
{
    uint32_t index = pin / 10;          // Index into the fsel array
    uint32_t shift = (pin % 10) * 3;    // Bit index of the given pin

    uint32_t selector = GPIO->fsel[index];

    selector &= ~(0b111 << shift);      // Clear the bits corresponding to the pin
    selector |= mode << shift;          // Set the required mode for the pin

    GPIO->fsel[index] = selector;
}

void gpio_set_pud(uint32_t pin, uint32_t pud)
{
    uint32_t index = pin / 32;
    uint32_t shift = pin % 32;

    GPIO->pud = pud;                    // Write the required control signal
    wait_cycles(150);
    GPIO->pudclk[index] = 1 << shift;   // Enable clock signal for the required pin
    wait_cycles(150);
    GPIO->pud = 0;                      // Remove the control signal
    GPIO->pudclk[index] = 0;            // Disable clock
}
