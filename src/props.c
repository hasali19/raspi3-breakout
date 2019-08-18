#include "props.h"
#include "mailbox.h"

#define MAILBOX_REQUEST         0x00000000
#define MAILBOX_SUCCESS         0x80000000
#define MAILBOX_ERROR           0x80000001

#define CHANNEL_PROP            8

#define TAG_GET_BOARD_SERIAL    0x00010004
#define TAG_END                 0

static volatile uint32_t __attribute__((aligned(16))) mailbox_buffer[16];

uint64_t get_board_serial(void)
{
    mailbox_buffer[0] = sizeof(mailbox_buffer);
    mailbox_buffer[1] = MAILBOX_REQUEST;

    mailbox_buffer[2] = TAG_GET_BOARD_SERIAL;
    mailbox_buffer[3] = 8;
    mailbox_buffer[4] = 0;
    mailbox_buffer[5] = 0;
    mailbox_buffer[6] = 0;

    mailbox_buffer[7] = TAG_END;

    uint32_t address = (uint32_t)(uintptr_t)&mailbox_buffer;

    asm volatile ("dmb sy");

    mailbox_write(CHANNEL_PROP, address);
    mailbox_read(CHANNEL_PROP);

    asm volatile ("dmb sy");

    return ((uint64_t)mailbox_buffer[6] << 32) | (mailbox_buffer[5]);
}
