.section ".text"

.global wait_cycles
wait_cycles:
    subs    x0, x0, #1
    b.ne    wait_cycles
    ret

.global get_sys_counter_frequency
get_sys_counter_frequency:
    mrs     x0, cntfrq_el0
    ret

.global get_sys_counter_count
get_sys_counter_count:
    mrs     x0, cntpct_el0
    ret

.global get_sys_counter_ms
get_sys_counter_ms:
    mrs     x1, cntfrq_el0
    mov     x0, #1000
    udiv    x1, x1, x0
    mrs     x0, cntpct_el0
    udiv    x0, x0, x1
    ret
