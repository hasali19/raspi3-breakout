#pragma once

#include <stdint.h>

#define GPIO_MODE_INPUT  0x0
#define GPIO_MODE_OUTPUT 0x1
#define GPIO_MODE_ALT0   0x4
#define GPIO_MODE_ALT1   0x5
#define GPIO_MODE_ALT2   0x6
#define GPIO_MODE_ALT3   0x7
#define GPIO_MODE_ALT4   0x3
#define GPIO_MODE_ALT5   0x2

#define GPIO_PUD_OFF       0
#define GPIO_PUD_PULL_DOWN 1
#define GPIO_PUD_PULL_UP   2

/**
 * Sets the selected mode for the given pin.
 */
void gpio_setup(uint32_t pin, uint32_t mode);

/**
 * Sets the pull up/down behaviour for the given pin.
 */
void gpio_set_pud(uint32_t pin, uint32_t pud);
