#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint16_t color_t;

/**
 * Creates a color_t with the given rgb values.
 */
#define RGB(red, green, blue)\
    ((color_t)((red << 11) | (green << 5) | (blue << 0)))

/**
 * Initialises a 32 bit framebuffer with the given width and height.
 */
bool fb_init(uint32_t width, uint32_t height);

/**
 * Clears the framebuffer with the given color.
 */
void fb_clear(color_t color);

/**
 * Draws a line between two points.
 */
void fb_draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, color_t color);

/**
 * Draws an outlined rectangle to the screen.
 */
void fb_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, color_t color);

/**
 * Draws a filled in rectangle to the screen.
 */
void fb_fill_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, color_t color);

/**
 * Draws an image specified by the GIMP header image format to the screen.
 */
void fb_draw_image(uint32_t x, uint32_t y, uint32_t width, uint32_t height, char* pixels);
