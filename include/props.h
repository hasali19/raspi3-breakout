#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MB_CLOCK_ARM    0x00000003

/**
 * Gets the board's serial number.
 */
uint64_t get_board_serial(void);

/**
 * Gets the frequency of the clock with the given id.
 */
uint32_t get_clock_rate(uint32_t clock_id);

/**
 * Sets the frequency of the clock with the given id to the specified value.
 *
 * Setting `skip_turbo` will prevent enabling of other turbo features when
 * setting ARM frequency above default.
 */
bool set_clock_rate(uint32_t clock_id, uint32_t rate, bool skip_turbo);

/**
 * Gets the maximum frequency of the clock with the given id.
 */
uint32_t get_max_clock_rate(uint32_t clock_id);
