#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Creates a color_t with the given rgb values.
 */
#define RGB(red, green, blue)\
    ((uint32_t)((0xFF << 24) | (red << 16) | (green << 8) | (blue << 0)))

/**
 * Initialises a 32 bit framebuffer with the given width and height.
 */
bool framebuffer_init(uint32_t width, uint32_t height);

/**
 * Clears the framebuffer with the given color.
 */
void framebuffer_clear(uint32_t color);
