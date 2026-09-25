ENTRY(_start)
SECTIONS
{
    . = 0x0;
    .text : {
        BYTE(0xE9);
        LONG(_start - 5);
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data : { *(.data) }
    /DISCARD/ : { *(.eh_frame) *(.comment) *(.note*) *(.eh_frame*) }
}