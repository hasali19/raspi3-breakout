#pragma once

#include <stdint.h>

/**
 * Gets the current value of the 1MHz system timer.
 */
uint64_t timer_get_ticks(void);

/**
 * Waits for the specified number of microseconds.
 */
void timer_wait(uint64_t us);
