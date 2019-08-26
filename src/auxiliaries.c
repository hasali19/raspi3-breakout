#include "peripherals/base.h"
#include "peripherals/auxiliaries.h"
#include "peripherals/gpio.h"

#include <stdint.h>

#include "assertions.h"

typedef struct
{
    uint32_t mini_uart : 1;
    uint32_t spi1 : 1;
    uint32_t spi2 : 1;
    uint32_t res : 29;
} aux_irq_t;

typedef struct
{
    uint32_t mini_uart : 1;
    uint32_t spi1 : 1;
    uint32_t spi2 : 1;
    uint32_t res : 29;
} aux_enables_t;

typedef struct __attribute__((packed))
{
    uint8_t value : 8;
    uint32_t res : 24;
} aux_mu_io_t;

typedef struct
{
    uint32_t enable_receive_interrupts : 1;
    uint32_t enable_transmit_interrupts : 1;
    uint32_t enable_lsi : 1;
    uint32_t enable_msi : 1;
    uint32_t res : 28;
} aux_mu_ier_t;

typedef struct
{
    uint32_t data_size : 2;
    uint32_t res1 : 4;
    uint32_t _break : 1;
    uint32_t dlab : 1;
    uint32_t res2 : 24;
} aux_mu_lcr_t;

typedef struct
{
    uint32_t res1 : 1;
    uint32_t rts : 1;
    uint32_t res2 : 30;
} aux_mu_mcr_t;

typedef struct
{
    uint32_t data_ready : 1;
    uint32_t receiver_overrun : 1;
    uint32_t res1 : 3;
    uint32_t transmitter_empty : 1;
    uint32_t transmitter_idle : 1;
    uint32_t res2 : 25;
} aux_mu_lsr_t;

typedef struct
{
    uint32_t receiver_enable : 1;
    uint32_t transmitter_enable : 1;
    uint32_t enable_rts : 1;
    uint32_t enable_cts : 1;
    uint32_t rts_flow_level : 2;
    uint32_t rts_assert_level : 1;
    uint32_t cts_assert_level : 1;
} aux_mu_cntl_t;

typedef struct
{
    uint32_t baudrate : 16;
    uint32_t res : 16;
} aux_mu_baud_t;

typedef struct
{
    aux_irq_t irq;
    aux_enables_t enables;
} aux_common_registers_t;

typedef struct
{
    aux_mu_io_t io;
    aux_mu_ier_t ier;
    uint32_t iir;
    aux_mu_lcr_t lcr;
    aux_mu_mcr_t mcr;
    aux_mu_lsr_t lsr;
    uint32_t msr;
    uint32_t scratch;
    aux_mu_cntl_t cntl;
    uint32_t stat;
    aux_mu_baud_t baud;
} aux_mu_registers_t;

ASSERT_SIZE(aux_irq_t, 4);
ASSERT_SIZE(aux_enables_t, 4);
ASSERT_SIZE(aux_mu_io_t, 4);
ASSERT_SIZE(aux_mu_ier_t, 4);
ASSERT_SIZE(aux_mu_lcr_t, 4);
ASSERT_SIZE(aux_mu_mcr_t, 4);
ASSERT_SIZE(aux_mu_lsr_t, 4);
ASSERT_SIZE(aux_mu_cntl_t, 4);
ASSERT_SIZE(aux_mu_baud_t, 4);

ASSERT_SIZE(aux_common_registers_t, 8);
ASSERT_SIZE(aux_mu_registers_t, 44);

#define AUX_COMMON ((volatile aux_common_registers_t*)(PERIPHERAL_BASE + 0x00215000))
#define AUX_MU     ((volatile aux_mu_registers_t*)(PERIPHERAL_BASE + 0x00215040))

void mini_uart_init(void)
{
    gpio_setup(14, GPIO_MODE_ALT5);
    gpio_setup(15, GPIO_MODE_ALT5);

    gpio_set_pud(14, GPIO_PUD_OFF);
    gpio_set_pud(15, GPIO_PUD_OFF);

    // Enable mini uart
    AUX_COMMON->enables = (aux_enables_t){ .mini_uart = 1 };

    AUX_MU->cntl = (aux_mu_cntl_t){ 0 };        // Disable receiver and transmitter (temporarily)

    AUX_MU->ier = (aux_mu_ier_t){ 0 };          // Disable receive and transmit interrupts
    AUX_MU->lcr = (aux_mu_lcr_t){ 0b11, 0 };    // Enable 8 bit mode
    AUX_MU->mcr = (aux_mu_mcr_t){ 0 };          // Set RTS line always high
    AUX_MU->baud = (aux_mu_baud_t){ 270 };      // Hard code baud rate to 115200

    // Enable receiver and transmitter
    AUX_MU->cntl = (aux_mu_cntl_t){
        .receiver_enable = 1,
        .transmitter_enable = 1,
    };
}

char mini_uart_read(void)
{
    // Wait until there is some data in the receive buffer
    while (!AUX_MU->lsr.data_ready) {}
    return AUX_MU->io.value;
}

char mini_uart_poll(void)
{
    if (AUX_MU->lsr.data_ready)
    {
        return AUX_MU->io.value;
    }

    return 0;
}

void mini_uart_write(char c)
{
    // Wait until there is space in the transmit buffer
    while (!AUX_MU->lsr.transmitter_empty) {}
    AUX_MU->io.value = c;
}

void mini_uart_write_string(const char* str)
{
    for (const char* c = str; *c; c++)
    {
        mini_uart_write(*c);
    }
}
