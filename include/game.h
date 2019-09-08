#pragma once

// #define FRAME_LENGTH 16666U     // 60 FPS
#define FRAME_LENGTH 33333U     // 30 FPS

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

/**
 * Initialises the game state.
 */
void game_init(void);

/**
 * Updates the game state.
 */
void game_update(double dt);

/**
 * Draws the game to the screen.
 */
void game_draw(void);
