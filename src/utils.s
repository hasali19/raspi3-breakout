.section ".text"

.global wait_cycles
wait_cycles:
    subs    x0, x0, #1
    b.ne    wait_cycles
    ret
