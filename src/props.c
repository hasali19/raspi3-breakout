#include "props.h"
#include "mailbox.h"

#include <stdarg.h>
#include <stddef.h>

#define MAILBOX_REQUEST         0x00000000
#define MAILBOX_SUCCESS         0x80000000
#define MAILBOX_ERROR           0x80000001

#define CHANNEL_PROP            8

#define TAG_GET_BOARD_SERIAL    0x00010004
#define TAG_GET_CLOCK_RATE      0x00030002
#define TAG_SET_CLOCK_RATE      0x00038002
#define TAG_GET_MAX_CLOCK_RATE  0x00030004
#define TAG_END                 0

static volatile uint32_t __attribute__((aligned(16))) mailbox_buffer[16];

uint32_t* mailbox_prop_message(size_t count, ...)
{
    if (count == 0)
    {
        return 0;
    }

    va_list args;
    va_start(args, count);

    mailbox_buffer[0] = sizeof(mailbox_buffer);
    mailbox_buffer[1] = MAILBOX_REQUEST;

    mailbox_buffer[count + 2] = TAG_END;

    for (size_t i = 0; i < count; i++)
    {
        mailbox_buffer[i + 2] = va_arg(args, uint32_t);
    }

    va_end(args);

    uint32_t address = (uint32_t)(uintptr_t)&mailbox_buffer;

    asm volatile ("dmb sy");
    mailbox_write(CHANNEL_PROP, address);
    uint32_t result = mailbox_read(CHANNEL_PROP);
    asm volatile ("dmb sy");

    bool success = result == address && mailbox_buffer[1] == MAILBOX_SUCCESS;

    if (!success)
    {
        return 0;
    }

    return (uint32_t*)&mailbox_buffer[2];
}

uint64_t get_board_serial(void)
{
    uint32_t* response = mailbox_prop_message(
        5, TAG_GET_BOARD_SERIAL, 8, 0, 0, 0);

    if (response == 0)
    {
        return 0;
    }

    return ((uint64_t)response[4] << 32) | (response[3]);
}

uint32_t get_clock_rate(uint32_t clock_id)
{
    uint32_t* response = mailbox_prop_message(
        5, TAG_GET_CLOCK_RATE, 8, 0, clock_id, 0);

    if (response == 0)
    {
        return 0;
    }

    return response[4];
}

bool set_clock_rate(uint32_t clock_id, uint32_t rate, bool skip_turbo)
{
    uint32_t* response = mailbox_prop_message(
        6, TAG_SET_CLOCK_RATE, 12, 0, clock_id, rate, skip_turbo);

    return response != 0 && response[4] == rate;
}

uint32_t get_max_clock_rate(uint32_t clock_id)
{
    uint32_t* response = mailbox_prop_message(
        5, TAG_GET_MAX_CLOCK_RATE, 8, 0, clock_id, 0);

    if (response == 0)
    {
        return 0;
    }

    return response[4];
}
