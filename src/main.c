#include "framebuffer.h"
#include "io.h"
#include "props.h"
#include "utils.h"

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

    uint64_t start = get_sys_counter_ms();

    if (framebuffer_init(800, 600))
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            framebuffer_clear(RGB(i, i, 0));
        }
    }

    uint64_t end = get_sys_counter_ms();

    printf("256 frames rendered in %d ms\n", end - start);

    while (1)
    {
        putchar(getchar());
    }
}
