#include "io.h"

#include "peripherals/auxiliaries.h"

char getchar(void)
{
    return mini_uart_read();
}

void putchar(char c)
{
    mini_uart_write(c);
}

/**
 * _putchar implementation for printf.
 */
void _putchar(char c)
{
    putchar(c);
}
