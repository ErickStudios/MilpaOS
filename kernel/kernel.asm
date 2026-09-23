    bits 32

; multiboot
section .multiboot
	; header multiboot
	align 4						; alineacion para multiboot
	dd 0x1BADB002				; codigo magico para indicar que es multiboot
	dd 0x0						; parametros de cargador de arranque
	dd - (0x1BADB002 + 0x00)	; checksum. m+f+c debe ser 0

extern c_main
extern pit_handler
global start
global isr_pit

start:
    cli
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

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp          ; Puntero al stack frame actual (regs_t* x)
    call pit_handler  ; C devuelve el nuevo ESP en EAX
    mov esp, eax      ; Cambiamos físicamente a la pila del siguiente proceso

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret