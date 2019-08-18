#include "io.h"
#include "props.h"

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

    while (1)
    {
        putchar(getchar());
    }
}
