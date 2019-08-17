#include <stddef.h>
#include <stdint.h>

#include "assertions.h"
#include "peripherals/base.h"

#define MEMORY_SIZE     0x40000000

#define PAGE_BITS       12
#define TABLE_BITS      9

#define PAGE_SIZE       (1 << PAGE_BITS)
#define TABLE_ENTRIES   (1 << TABLE_BITS)

#define BLOCK_ATTRIBUTES (0x1 << 0) | (0x1 << 10)

typedef uint64_t descriptor_t;

ASSERT_SIZE(descriptor_t, 8);

/**
 * Calculates the table index for a virtual address at the given level.
 */
#define VIRT_TBL_INDEX(addr, level)\
    ((addr) >> (PAGE_BITS + (3 - (level)) * TABLE_BITS))

/**
 * Creates a descriptor pointing to the next level table.
 */
#define TABLE_DESCRIPTOR(addr)\
    ((descriptor_t)(((uint64_t)addr & 0xFFFFFFFFF000) | 0b11))

/**
 * Creates a descriptor pointing to a memory block with the given physical
 * address and lower attributes.
 */
#define BLOCK_DESCRIPTOR(addr, attrs)\
    ((descriptor_t)(((uint64_t)addr & 0xFFFFFFFFF000) | ((attrs) & 0xFFC) | 0x1))

static descriptor_t __attribute__((aligned(PAGE_SIZE))) l0_table[TABLE_ENTRIES] = { 0 };
static descriptor_t __attribute__((aligned(PAGE_SIZE))) l1_table[TABLE_ENTRIES] = { 0 };
static descriptor_t __attribute__((aligned(PAGE_SIZE))) l2_table[TABLE_ENTRIES] = { 0 };

void mmu_create_tables(void)
{
    l0_table[0] = TABLE_DESCRIPTOR(l1_table);
    l1_table[0] = TABLE_DESCRIPTOR(l2_table);

    size_t i = 0;

    // Create entries for memory from 0x0 to peripheral base
    for (; i < VIRT_TBL_INDEX(PERIPHERAL_BASE, 2); i++)
    {
        uint64_t physical_address = i << (PAGE_BITS + TABLE_BITS);
        l2_table[i] = BLOCK_DESCRIPTOR(physical_address, BLOCK_ATTRIBUTES | (0x1 << 2));
    }

    // Create entries for peripherals
    for (; i < VIRT_TBL_INDEX(MEMORY_SIZE, 2); i++)
    {
        uint64_t physical_address = i << (PAGE_BITS + TABLE_BITS);
        l2_table[i] = BLOCK_DESCRIPTOR(physical_address, BLOCK_ATTRIBUTES | (0x0 << 2));
    }
}

void _mmu_init(descriptor_t*);

void mmu_init(void) {
    _mmu_init(l0_table);
}
