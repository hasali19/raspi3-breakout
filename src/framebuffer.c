#include "framebuffer.h"
#include "assertions.h"
#include "mailbox.h"

#include <stddef.h>

#define MAILBOX_REQUEST 0x00000000
#define MAILBOX_SUCCESS 0x80000000

#define CHANNEL_PROP    8

#define TAG_ALLOCATE_BUFFER         0x00040001
#define TAG_SET_PHYS_WIDTH_HEIGHT   0x00048003
#define TAG_SET_VIRT_WIDTH_HEIGHT   0x00048004
#define TAG_SET_DEPTH               0x00048005
#define TAG_SET_PIXEL_ORDER         0x00048006
#define TAG_GET_PITCH               0x00040008
#define TAG_SET_VIRT_OFFSET         0x00048009
#define TAG_END                     0x00000000

static uint32_t __attribute__((aligned(16))) mailbox_buffer[35];

static struct
{
    uint32_t* ptr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
} framebuffer;

bool framebuffer_init(uint32_t width, uint32_t height)
{
    mailbox_buffer[0] = sizeof(mailbox_buffer);
    mailbox_buffer[1] = MAILBOX_REQUEST;

    mailbox_buffer[2] = TAG_ALLOCATE_BUFFER;
    mailbox_buffer[3] = 8;
    mailbox_buffer[4] = 0;
    mailbox_buffer[5] = 4096;
    mailbox_buffer[6] = 0;

    mailbox_buffer[7] = TAG_SET_PHYS_WIDTH_HEIGHT;
    mailbox_buffer[8] = 8;
    mailbox_buffer[9] = 0;
    mailbox_buffer[10] = width;
    mailbox_buffer[11] = height;

    mailbox_buffer[12] = TAG_SET_VIRT_WIDTH_HEIGHT;
    mailbox_buffer[13] = 8;
    mailbox_buffer[14] = 0;
    mailbox_buffer[15] = width;
    mailbox_buffer[16] = height;

    mailbox_buffer[17] = TAG_SET_DEPTH;
    mailbox_buffer[18] = 4;
    mailbox_buffer[19] = 0;
    mailbox_buffer[20] = 32;

    mailbox_buffer[21] = TAG_SET_PIXEL_ORDER;
    mailbox_buffer[22] = 4;
    mailbox_buffer[23] = 0;
    mailbox_buffer[24] = 1;

    mailbox_buffer[25] = TAG_GET_PITCH;
    mailbox_buffer[26] = 4;
    mailbox_buffer[27] = 0;
    mailbox_buffer[28] = 0;

    mailbox_buffer[29] = TAG_SET_VIRT_OFFSET;
    mailbox_buffer[30] = 8;
    mailbox_buffer[31] = 8;
    mailbox_buffer[32] = 0;
    mailbox_buffer[33] = 0;

    mailbox_buffer[34] = TAG_END;

    uint32_t mailbox_buffer_addr = (uint32_t)(uintptr_t)&mailbox_buffer;

    asm volatile ("dmb sy");
    mailbox_write(8, mailbox_buffer_addr);
    uint32_t result = mailbox_read(CHANNEL_PROP);
    asm volatile ("dmb sy");

    if (result != mailbox_buffer_addr || mailbox_buffer[1] != MAILBOX_SUCCESS)
    {
        return false;
    }

    framebuffer.ptr = (uint32_t*)(uintptr_t)(mailbox_buffer[5] & 0x3FFFFFFF);

    if (framebuffer.ptr == NULL)
    {
        return false;
    }

    framebuffer.width  = mailbox_buffer[10];
    framebuffer.height = mailbox_buffer[11];
    framebuffer.pitch  = mailbox_buffer[28] / 4;

    return true;
}

void framebuffer_clear(uint32_t color)
{
    uint32_t* ptr = framebuffer.ptr;

    if (ptr == NULL)
    {
        return;
    }

    for (int y = 0; y < framebuffer.height; y++)
    {
        for (int x = 0; x < framebuffer.width; x++)
        {
            *(ptr++) = color;
        }

        ptr += framebuffer.pitch - framebuffer.width;
    }
}