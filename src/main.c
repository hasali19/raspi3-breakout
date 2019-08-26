#include "framebuffer.h"
#include "game.h"
#include "io.h"
#include "props.h"
#include "peripherals/timer.h"

void set_arm_max_clock_rate(void)
{
    uint32_t max_rate = get_max_clock_rate(MB_CLOCK_ARM);
    set_clock_rate(MB_CLOCK_ARM, max_rate, true);
}

void main(void)
{
    set_arm_max_clock_rate();

    printf("Serial number: 0x%016x\n", get_board_serial());
    printf("ARM clock rate: %d MHz\n", get_clock_rate(0x000000003) / 1000000);

    if (!fb_init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf("Failed to initialise framebuffer\n");
        while (1);  // hang
    }

    const double dt = FRAME_LENGTH / 1000000.0;

    game_init();

    while (1)
    {
        uint64_t start = timer_get_ticks();

        game_update(dt);
        game_draw();

        uint64_t end = timer_get_ticks();
        uint64_t diff = end - start;

        if (diff < FRAME_LENGTH)
        {
            timer_wait(FRAME_LENGTH - diff);
        }
    }
}
