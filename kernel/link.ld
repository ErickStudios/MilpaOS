/*
 * link.ld
 */

OUTPUT_FORMAT(elf32-i386)

ENTRY(start)

SECTIONS
{
    /* actual */
	. = 0x100000;
    .multiboot : { KEEP(*(.multiboot)) }
    KernelStart = .;
	.text : {*(.text)}
    KernelEnd = .;
	.data : {*(.data)}
	.bss  : {
    __bss_start = .;
    *(.bss*) *(COMMON)
    __bss_end = .;
    }

    . = ALIGN(4);
    .kstack (NOLOAD) :
    {
        __kstack_start = .;
        . = . + 0x4000;
        . = ALIGN(4);
        __kstack_end = .;
    }

    . = ALIGN(4);
    _heap_start = .;
    . += 0x8000000;
    _heap_end = .;
}
