#include "io.h"
#include "props.h"

void main(void)
{
    printf("Serial number: 0x%016x\n", get_board_serial());

    while (1)
    {
        putchar(getchar());
    }
}
