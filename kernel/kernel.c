#include "../lib/abstract.h"
#include "../lib/stdlib.h"
#include "heap.h"
#include "pipe.h"
#include "ports.h"
#include "keyio.h"
#include "idt.h"
#include "pic.h"
#include "termdef.h"
#include "multibt.h"
#include "vga.h"
#include "disk.h"
#include "font.h"
#include "stackfall.h"
#include "termios.h"
#include "mouse.h"
#include "gafas.h"
#include "msg.h"

void proc1() {
    vprintk("hello world\n");
    msghandler_t abc = request_msg("test1_fi", 0);
    vprintk("sended\n");
    join_msg(abc);
    vprintk("responded\n");

    while (1) {
        char ky = getc();
        vputc(ky);
    }
}

void proc2() {
    vprintk("foo bar\n");
    while (1) {
        msghandler_t xd = findfirst_msg("test1_fi");

        if (xd != INVALID_MSG_ID) {
            vprintk("recived\n");
        }
        respond_msg(xd, 0);
    }
}

void render() {
    abstract_t* fb =(abstract_t*)back_buffer;

    for (abstract_t i = 0; i < (globInf->framebuffer_height*globInf->framebuffer_width); i++)
    {
        fb[i] = 0x00008888;
    }

    terminal_column = 0;
    terminal_row = 0;

    terminal_color = 0x9E;

    printk("Welcome to the MilpaOS kernel, this is the multiple virtual terminal desktop worckspace\n");

    for (int i = 0; i < 80*25; i++)
    {
        *((abssmall_t*)(&terminal_buffer[i]) + 1) = 0x1E;
    }

    for (int i = 0; i < MAX_BG_PROCESS; i++)
    {
        if (stats[i].free == 1) {
            int x = 5 + (i * (ROWS_OF_VIRT + 3));
            int y = 5;
            // stats[i].virtus.buf = 15*7 buf

            int w = ROWS_OF_VIRT;
            int h = COLS_OF_VIRT;
            
            int vrt_i = 0;

            for (int sy = (y - 1); sy < (y + h + 1); sy++)
            {
                for (int sx = (x - 1); sx < (x + w + 1); sx++)
                {
                    writeTermBuffer(sx, sy, 0x2020);
                }
            }

            for (int sy = y; sy < (y + h); sy++)
            {
                for (int sx = x; sx < (x + w); sx++)
                {
                    writeTermBuffer(sx, sy, stats[i].virtus.buf[vrt_i]);
                    vrt_i++;
                }
            }
        }
    }
}

void proc0() {
    while (true) {
        //render();
    }
}

void c_main(abstract_t magic, abstract_t mbi_addr) {
    multiboot_info_t *mbi =
        (multiboot_info_t *)mbi_addr;

    if (magic != 0x2BADB002)
        return;

    if (!(mbi->flags & (1 << 12)))
        return;

    globInf = mbi;

    initTerm();

    init_heap();
    remap_pic();

    idt_set_gate(32, (abstract_t)isr_pit, 0x10, 0x8E);
    idt_install();

    init_backbuffer();

    running_proc = 0;
    for (int i = 0; i < MAX_BG_PROCESS; i++)
    {
        stats[i].free = 0;
    }

    proalloc((abstract_t)&proc0);
    proalloc((abstract_t)&proc1);
    proalloc((abstract_t)&proc2);

    init_mouse();

    init_pit(50);

    asm volatile("sti");

    while (true) {
        render();
        popback();
    }
}