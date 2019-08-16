.section ".text.vectors"

.macro  vector  handler
.balign 0x80
    b       \handler
.endm

.global vector_table
.balign 0x800
vector_table:
    // Current EL with SP0
    vector  error_hang
    vector  error_hang
    vector  error_hang
    vector  error_hang

    // Current EL with SPx
    vector  handle_synchronous
    vector  handle_irq
    vector  handle_fiq
    vector  handle_serror

    // Lower EL using AArch64
    vector  error_hang
    vector  error_hang
    vector  error_hang
    vector  error_hang

    // Lower EL using AArch32
    vector  error_hang
    vector  error_hang
    vector  error_hang
    vector  error_hang

handle_synchronous:
    adr     x0, sync_message
    b       handle_exception

handle_irq:
    adr     x0, irq_message
    b       handle_exception

handle_fiq:
    adr     x0, fiq_message
    b       handle_exception

handle_serror:
    adr     x0, serror_message
    b       handle_exception

handle_exception:
    bl      mini_uart_write_string
    b       error_hang

error_hang:
    wfe
    b       error_hang

.section ".rodata"

sync_message:
    .ascii "Synchronous exception occurred\n\0"

irq_message:
    .ascii "IRQ exception occurred\n\0"

fiq_message:
    .ascii "FIQ exception occurred\n\0"

serror_message:
    .ascii "SError exception occurred\n\0"
