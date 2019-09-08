#include "game.h"
#include "framebuffer.h"

#include <stdbool.h>

#include "peripherals/auxiliaries.h"
#include "peripherals/rand.h"

#include "circle.h"

#define BLACK RGB(  0,   0,   0)
#define WHITE RGB(255, 255, 255)

#define PADDLE_WIDTH    150
#define PADDLE_HEIGHT   20
#define PADDLE_COLOR    WHITE
#define PADDLE_SPEED    400

#define BRICK_WIDTH     80
#define BRICK_HEIGHT    40
#define BRICK_ROWS      ((SCREEN_HEIGHT / 2) / BRICK_HEIGHT)
#define BRICK_COLS      (SCREEN_WIDTH / BRICK_WIDTH)
#define BRICK_COUNT     (BRICK_ROWS * BRICK_COLS)

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

static color_t bricks[BRICK_COUNT];

void game_init(void)
{
    paddle_position = (SCREEN_WIDTH / 2) - (PADDLE_WIDTH / 2);

    ball.position.x = SCREEN_WIDTH / 2;
    ball.position.y = SCREEN_HEIGHT - PADDLE_HEIGHT - (circle_height / 2);

    ball.velocity.x = 50;
    ball.velocity.y = -200;

    for (int i = 0; i < BRICK_COUNT; i++)
    {
        uint8_t r = rand_generate(171, 255);
        uint8_t g = rand_generate(86, 170);
        uint8_t b = rand_generate(0, 85);

        bricks[i] = RGB(r, g, b);
    }
}

void ball_update(double dt)
{
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
        if (ball.position.x >= paddle_position
            && ball.position.x <= paddle_position + PADDLE_WIDTH)
        {
            ball.position.y = SCREEN_HEIGHT - PADDLE_HEIGHT - (circle_height / 2);
            ball.velocity.y *= -1;
        }
    }
}

void paddle_update(double dt, char input)
{
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

void game_update(double dt)
{
    char input = mini_uart_poll();

    ball_update(dt);
    paddle_update(dt, input);
}

void game_draw()
{
    fb_clear(BLACK);

    fb_fill_rect(paddle_position, SCREEN_HEIGHT - PADDLE_HEIGHT,
        PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_COLOR);

    for (int i = 0; i < BRICK_ROWS; i++)
    {
        for (int j = 0; j < BRICK_COLS; j++)
        {
            color_t brick = bricks[i * BRICK_COLS + j];
            if (brick)
            {
                fb_fill_rect(j * BRICK_WIDTH, i * BRICK_HEIGHT,
                    BRICK_WIDTH, BRICK_HEIGHT, brick);
            }
        }
    }

    fb_draw_image(ball.position.x - (circle_width / 2),
        ball.position.y - (circle_height / 2),
        circle_width, circle_height, circle_data);
}
