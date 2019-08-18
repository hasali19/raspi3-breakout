#pragma once

#include <stdint.h>

/**
 * Waits for the specified number of cycles.
 */
void wait_cycles(uint64_t cycles);

/**
 * Gets the frequency of the system counter (cntfrq_el0).
 */
uint64_t get_sys_counter_frequency(void);

/**
 * Gets the physical count value of the system counter (cntpct_el0).
 */
uint64_t get_sys_counter_count(void);

/**
 * Gets the value of the system counter (cntpct_el0) in milliseconds.
 */
uint64_t get_sys_counter_ms(void);
