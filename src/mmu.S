.section ".text"

.set MAIR_EL1_VAL,  (0x00 << 0) | /* Device-nGnRnE memory */ \
                    (0x44 << 8)   /* Normal memory, outer non-cacheable, inner non-cacheable */

.set TCR_EL1_VAL,   (16 << 0) |  /* T0SZ: Region size 2^(64 - val) */ \
                    (0x00 << 14) /* TG0: 4KB granule size */

.global _mmu_init
_mmu_init:
    // Set base address of l0 translation table
    msr ttbr0_el1, x0

    mov x0, TCR_EL1_VAL
    msr tcr_el1, x0

    // Configure memory attribute types
    mov x0, MAIR_EL1_VAL
    msr mair_el1, x0

    // Enable address translation
    mrs x0, sctlr_el1
    orr x0, x0, #0x1
    msr sctlr_el1, x0

    ret
