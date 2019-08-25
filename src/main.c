#include "framebuffer.h"
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

    uint64_t start = timer_get_ticks();

    if (fb_init(800, 600))
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            fb_clear(RGB(i, 0, i));

            fb_draw_rect(100 + i, 100, 100, 100, RGB(255, 255, 255));
            fb_fill_rect(100 + i, 400, 100, 100, RGB(255, 255, 255));
        }
    }

    uint64_t end = timer_get_ticks();

    printf("256 frames rendered in %d ms\n", (end - start) / 1000);

    while (1)
    {
        putchar(getchar());
    }
}
