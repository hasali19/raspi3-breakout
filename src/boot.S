.section ".text.boot"

.set HCR_EL2_VAL,   (1 << 31)  /* RW: Enable aarch64 execution state for el1 */

.set SCTLR_EL1_VAL, (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11) |  /* Reserved */ \
                    (0 << 25) |     /* EE: Use little endian data accesses at el1 */ \
                    (0 << 24) |     /* E0E: Use little endian data accesses at el0 */ \
                    (0 << 12) |     /* I: Disable instruction cache */ \
                    (0 << 2)  |     /* C: Disable data cache */ \
                    (0 << 0)        /* M: Disable MMU */

.set SPSR_EL2_VAL,  (0xF << 6) |    /* F,I,A,D: Mask all interrupts */ \
                    (5 << 0)        /* M: Return to el1h */

.global _start
_start:
    mov x0, HCR_EL2_VAL
    msr hcr_el2, x0

    ldr x0, =SCTLR_EL1_VAL
    msr sctlr_el1, x0

    mov x0, SPSR_EL2_VAL
    msr spsr_el2, x0

    // Set exception return address
    adr x0, el1_entry
    msr elr_el2, x0

    eret

el1_entry:
    // Clear the bss section
    ldr     x0, =__bss_begin__
    ldr     x1, =__bss_end__
    cmp     x0, x1
    b.cs    2f
1:  str     xzr, [x0], #8
    cmp     x1, x0
    b.hi    1b
2:
    // Set exception vector table
    adr     x0, vector_table
    msr     vbar_el1, x0

setup_stack:
    // Set the stack pointer
    ldr     x0, =__stack__
    mov     sp, x0

_main:
    // Initialise mmu
    bl      mmu_create_tables
    bl      mmu_init

    // Initialise mini uart
    bl      mini_uart_init

    // Jump to c code
    bl      main
    b       halt

halt:
    wfe
    b       halt
