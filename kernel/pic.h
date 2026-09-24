#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

static void pic_unmask_irq1(void) {
    uabssmall_t imr = inb(0x21);      // PIC maestro
    outb(0x21, imr & ~0x01);      // desmascara bit 1 (IRQ1)
}

extern void isr_pit();

void remap_pic() {
    outb(0x20, 0x11); // PIC maestro
    outb(0xA0, 0x11); // PIC esclavo

    // ICW2: offset de vectores
    outb(0x21, 0x20); // maestro: 0x20
    outb(0xA1, 0x28); // esclavo: 0x28

    // ICW3: encadenamiento
    outb(0x21, 0x04); // esclavo conectado al IRQ2 del maestro
    outb(0xA1, 0x02); // este esclavo es línea 2

    // ICW4: modo 8086
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Enmascarar todo inicialmente
    outb(0x21, 0b11111110);
    outb(0xA1, 0xFF);
}

void init_pit(abstract_t frequency) {
    abslittl_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    outb(0x40, (uabssmall_t)(divisor & 0xFF));
    outb(0x40, (uabssmall_t)((divisor >> 8) & 0xFF));
}

volatile abstract_t system_ticks = 0;

#include "regs.h"

#define MAX_BG_PROCESS 50
#define ROWS_OF_VIRT 26
#define COLS_OF_VIRT 20

typedef struct {
    abslittl_t buf[ROWS_OF_VIRT*COLS_OF_VIRT];
    abstract_t row;
    abstract_t column;
    uabssmall_t color;
} VirtTerm;

typedef struct {
    abstract_t stack[512];
    regs_t stats;
    bool_t free;
    VirtTerm virtus;
} Process;

Process stats[MAX_BG_PROCESS];

struct SysInfo {
    int ver; // X.XX 100 = 1.00, 320 = 3.20
} SystemInfo ;

void syscall(regs_t* r) {
    switch (r->edi)
    {
    case 0:
        char* x = malloc(r->ecx);
        r->eax = (abstract_t)x;
        break;
    
    default:
        break;
    }
}

Process* proalloc(abstract_t fn) {
    for (int i = 0; i < MAX_BG_PROCESS; i++) {
        if (stats[i].free == 0) {
            abstract_t* sp = (abstract_t*)((abstract_t)&stats[i].stack[512] & ~0xF);

            *(--sp) = 0x202;
            *(--sp) = 0x10;
            *(--sp) = fn;

            for (int j = 0; j < 8; j++) {
                *(--sp) = 0; 
            }

            *(--sp) = 0x18;
            *(--sp) = 0x18;
            *(--sp) = 0x18;
            *(--sp) = 0x18;

            stats[i].stats.esp = (abstract_t)sp;

            stats[i].virtus.color = 0x1F;
            stats[i].virtus.column = 0;
            stats[i].virtus.row = 0;
            for(int j=0; j<ROWS_OF_VIRT*COLS_OF_VIRT; j++) stats[i].virtus.buf[j] = 0x0020;

            stats[i].free = 1;
            return &stats[i];
        }
    }
    return 0;
}

int running_proc = 0;

abstract_t pit_handler(regs_t* x) {
    system_ticks++;

    stats[running_proc].stats.esp = (abstract_t)x;

    int next_proc = running_proc;
    int found = 0;

    for (int i = 0; i < MAX_BG_PROCESS; i++) {
        next_proc = (next_proc + 1) % MAX_BG_PROCESS;
        if (stats[next_proc].free == 1) {
            found = 1;
            break;
        }
    }

    if (!found) {
        next_proc = running_proc;
    }

    running_proc = next_proc;
    Process* next = &stats[running_proc];

    outb(0x20, 0x20);

    return next->stats.esp;
}