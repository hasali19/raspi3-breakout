ENTRY(_start)

SECTIONS
{
    .text.boot 0x80000 : {
        KEEP(*(.text.boot))
    }

    /DISCARD/ : {
        *(*)
    }
}
