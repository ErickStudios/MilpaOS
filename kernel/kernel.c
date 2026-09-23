#include "../lib/abstract.h"
#include "heap.h"
#include "pipe.h"
#include "ports.h"
#include "keyio.h"
#include "idt.h"
#include "pic.h"
#include "vga.h"
#include "disk.h"

void __stack_chk_fail_local()
{
    while (1)
    {
        __asm__("cli");
        __asm__("hlt");
    }
}

void __stack_chk_fail()
{
    __stack_chk_fail_local();
}

extern void isr_pit();

abstract_t readl(char *ot)
{
    abstract_t it = 0;
    while (true)
    {
        char ky = getc();
        putc(ky);
        if (ky == '\b')
        {
            ot[it--] = 0;
        }
        else if (ky == '\n')
        {
            ot[it] = 0;
            return it;
        }
        else
        {
            ot[it++] = ky;
        }
    }

    return 0;
}

static void pic_unmask_irq1(void) {
    uabssmall_t imr = inb(0x21);      // PIC maestro
    outb(0x21, imr & ~0x01);      // desmascara bit 1 (IRQ1)
}

void proc1() {
    vprintk("hello world\n");
    while (1) {
        char ky = getc();
        vputc(ky);
    }
}

void proc2() {
    vprintk("foo bar\n");
    while (1);
}

void render() {
    terminal_column = 0;
    terminal_row = 0;

    for (int i = 0; i < 80*25; i++)
    {
        terminal_buffer[i] = 0x1E20;
    }

    printk("Welcome to the MilpaOS kernel, this is the multiple virtual terminal desktop worckspace\n");

    for (int i = 0; i < 80*25; i++)
    {
        *((abssmall_t*)(&terminal_buffer[i]) + 1) = 0x1E;
    }

    for (int i = 0; i < MAX_BG_PROCESS; i++)
    {
        if (stats[i].free == 1) {
            int x = 5 + (i * 17);
            int y = 5;
            // stats[i].virtus.buf = 15*7 buf

            int w = 15;
            int h = 7;
            
            int vrt_i = 0;
            for (int sy = y; sy < (y + h); sy++)
            {
                for (int sx = x; sx < (x + w); sx++)
                {
                    terminal_buffer[sy*80+sx] = stats[i].virtus.buf[vrt_i];
                    vrt_i++;
                }
            }
        }
    }
}

void proc0() {
    while (true) {
        render();
    }
}

void c_main() {
    init_heap();
    remap_pic();

    idt_set_gate(32, (abstract_t)isr_pit, 0x08, 0x8E);
    idt_install();

    running_proc = 0;
    for (int i = 0; i < MAX_BG_PROCESS; i++)
    {
        stats[i].free = 0;
    }

    proalloc((abstract_t)&proc0);
    proalloc((abstract_t)&proc1);
    proalloc((abstract_t)&proc2);

    init_pit(50);
    
    asm volatile("sti");

    while (true) {
        render();
    }
}