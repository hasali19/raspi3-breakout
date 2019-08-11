ENTRY(_start)

SECTIONS
{
    .text.boot 0x80000 : {
        KEEP(*(.text.boot))
    }

    .text : {
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        . = ALIGN(8);
        __bss_begin__ = .;
        *(.bss)
        __bss_end__ = .;
    }

    .stack : {
        . = ALIGN(16);
        . = . + 0x00100000;     /* 1MB stack */
        __stack__ = .;
    }

    /DISCARD/ : {
        *(*)
    }
}
