    bits 32

; multiboot
section .multiboot
	; header multiboot
	align 4						; alineacion para multiboot
    dd 0x1BADB002              ; magic
    dd (1 << 0) | (1 << 1) | (1 << 2)
    dd -(0x1BADB002 + ((1 << 0) | (1 << 1) | (1 << 2)))

    times 5 dd 0

    dd 0
    dd 800
    dd 600
    dd 32


extern c_main
extern pit_handler
global start
global isr_pit
extern __kstack_end

start:
    cli
    mov esp, __kstack_end
    push ebx
    push eax
    call c_main
    hlt
    ret

kata dd 0

isr_pit:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x18
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call pit_handler
    mov esp, eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret