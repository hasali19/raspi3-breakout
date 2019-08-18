#include "io.h"
#include "mailbox.h"

static volatile uint32_t __attribute__((aligned(16))) mailbox_buffer[16];

void main(void)
{
    printf("Hello, world!\n");

    mailbox_buffer[0] = sizeof(mailbox_buffer);
    mailbox_buffer[1] = 0;

    mailbox_buffer[2] = 0x00010004;
    mailbox_buffer[3] = 8;
    mailbox_buffer[4] = 0;
    mailbox_buffer[5] = 0;
    mailbox_buffer[6] = 0;

    mailbox_buffer[7] = 0;

    uint32_t address = (uint32_t)(uintptr_t)&mailbox_buffer;
    mailbox_write(8, address);
    mailbox_read(8);

    printf("Serial number: 0x%016x\n", ((uint64_t)mailbox_buffer[6] << 32) | (mailbox_buffer[5]));

    while (1)
    {
        putchar(getchar());
    }
}
