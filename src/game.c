#include "game.h"
#include "framebuffer.h"

#define BLACK RGB(  0,   0,   0)
#define WHITE RGB(255, 255, 255)

void game_init(void)
{
}

void game_update(double dt)
{
}

void game_draw()
{
    fb_clear(BLACK);

    fb_fill_rect(0, SCREEN_HEIGHT - 20, 150, 20, WHITE);
}
