.section ".text.boot"
.global _start
_start:

halt:
    wfe
    b   halt
