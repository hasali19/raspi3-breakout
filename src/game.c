#include "game.h"
#include "framebuffer.h"

#include "peripherals/auxiliaries.h"

#include "circle.h"

#define BLACK RGB(  0,   0,   0)
#define WHITE RGB(255, 255, 255)

#define PADDLE_WIDTH    150
#define PADDLE_HEIGHT   20
#define PADDLE_COLOR    WHITE
#define PADDLE_SPEED    400

typedef struct
{
    float x;
    float y;
} vec2f_t;

static float paddle_position;

static struct
{
    vec2f_t position;
    vec2f_t velocity;
} ball;

void game_init(void)
{
    paddle_position = (SCREEN_WIDTH / 2) - (PADDLE_WIDTH / 2);

    ball.position.x = SCREEN_WIDTH / 2;
    ball.position.y = SCREEN_HEIGHT - PADDLE_HEIGHT - (circle_height / 2);

    ball.velocity.x = 50;
    ball.velocity.y = -200;
}

void game_update(double dt)
{
    char input = mini_uart_poll();

    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;

    if (ball.position.y <= (circle_height / 2))
    {
        ball.velocity.y *= -1;
    }

    if (ball.position.x >= SCREEN_WIDTH - (circle_width / 2)
        || ball.position.x <= (circle_width / 2))
    {
        ball.velocity.x *= -1;
    }

    if (ball.position.y >= SCREEN_HEIGHT - PADDLE_HEIGHT
        && ball.position.y <= SCREEN_HEIGHT - (PADDLE_HEIGHT / 2))
    {
        if (ball.position.x >= paddle_position && ball.position.x <= paddle_position + PADDLE_WIDTH)
        {
            ball.position.y = SCREEN_HEIGHT - PADDLE_HEIGHT - (circle_height / 2);
            ball.velocity.y *= -1;
        }
    }

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

    fb_draw_image(ball.position.x - (circle_width / 2), ball.position.y - (circle_height / 2),
        circle_width, circle_height, circle_data);
}
