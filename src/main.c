#include "peripherals/aux.h"

void main(void)
{
    mini_uart_init();
    mini_uart_write_string("Hello, world!\n");

    while (1)
    {
        mini_uart_write(mini_uart_read());
    }
}
