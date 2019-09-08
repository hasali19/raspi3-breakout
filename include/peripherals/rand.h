#pragma once

#include <stdint.h>

/**
 * Initialises the hardware random number generator.
 */
void rand_init(void);

/**
 * Generates a random number between min and max.
 */
uint32_t rand_generate(uint32_t min, uint32_t max);
