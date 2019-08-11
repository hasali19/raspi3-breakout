.section ".text.boot"
.global _start
_start:
    // Clear the bss section
    adr     x0, __bss_begin__
    adr     x1, __bss_end__
    cmp     x0, x1
    b.cs    setup_stack
1:  str     xzr, [x0], #8
    cmp     x1, x0
    b.hi    1b

setup_stack:
    // Set the stack pointer
    ldr     x0, =__stack__
    mov     sp, x0

_main:
    // Jump to c code
    bl      main
    b       halt

halt:
    wfe
    b       halt
