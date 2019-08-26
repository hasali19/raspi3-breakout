#include "game.h"
#include "framebuffer.h"

#include "peripherals/auxiliaries.h"

#define BLACK RGB(  0,   0,   0)
#define WHITE RGB(255, 255, 255)

#define PADDLE_WIDTH    150
#define PADDLE_HEIGHT   20
#define PADDLE_COLOR    WHITE
#define PADDLE_SPEED    400

static float paddle_position;

void game_init(void)
{
    paddle_position = (SCREEN_WIDTH / 2) - (PADDLE_WIDTH / 2);
}

void game_update(double dt)
{
    char input = mini_uart_poll();

    if (input == 'a')
    {
        paddle_position -= PADDLE_SPEED * dt;
    }
    else if (input == 'd')
    {
        paddle_position += PADDLE_SPEED * dt;
    }

    if (paddle_position > SCREEN_WIDTH - PADDLE_WIDTH)
    {
        paddle_position = SCREEN_WIDTH - PADDLE_WIDTH;
    }
    else if (paddle_position < 0)
    {
        paddle_position = 0;
    }
}

void game_draw()
{
    fb_clear(BLACK);

    fb_fill_rect(paddle_position, SCREEN_HEIGHT - PADDLE_HEIGHT,
        PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_COLOR);
}
