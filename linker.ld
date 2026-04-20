ENTRY(_start)

SECTIONS {
    . = 0x1000;

    .text : ALIGN(4) {
        *(.text._start)
        *(.text)
    }

    .data : ALIGN(4) {
        *(.data)
    }

    .bss : ALIGN(4) {
        *(.bss)
    }
}
