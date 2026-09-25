#include "../lib/abstract.h"
#include "../lib/stdlib.h"
#include "heap.h"
#include "pipe.h"
#include "ports.h"
#include "keyio.h"
#include "idt.h"
#include "msg.h"
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
#include "fstraps.h"
#include "server.h"
#include "gui.h"

void proc1(server_t *serv) {
    serv->printCon("hello world\n");
    msghandler_t abc = serv->createRequest("test1_fi", 0);
    serv->printCon("sended\n");
    serv->joinRequest(abc);
    serv->printCon("responded\n");

    while (1) {
        //char ky = serv->inputCon();
        //serv->teletypeCon(ky);
    }
}

void proc2(server_t *serv) {
    serv->printCon("foo bar\n");
    while (1) {
        (*serv->structCon.color)+=2;
        //*serv->structCon.row = 0;
        //*serv->structCon.column = 0;
        serv->printCon("foo bar");
    }
    /*while (1) {
        msghandler_t xd = serv->findFirstRequest("test1_fi");

        if (xd != INVALID_MSG_ID) {
            serv->printCon("recived\n");
            serv->respondRequest(xd, 0);
        }
    }*/
}

void proc0(server_t *serv) {
    while (true) {
        //render();
    }
}

/*
    printk(fil);
    putc('\n');

    popback();

    abstract_t start_sect = Fat16ClusterToLBA(&inf16, bpb, fil->cluster_lo);

    ata_read_sector(start_sect, s0);

    printk(s0);

    popback();*/

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

    init_msg_server();

    server_t ks;
    init_functions(&ks);

    idt_set_gate(32, (abstract_t)isr_pit, 0x10, 0x8E);
    idt_install();

    init_backbuffer();

    running_proc = 0;
    for (int i = 0; i < MAX_BG_PROCESS; i++)
    {
        stats[i].free = 0;
    }

    proalloc((abstract_t)&proc0);
    //proalloc((abstract_t)&proc1);
    proalloc((abstract_t)&proc2);

    init_mouse();

    Fat16Info spc;
    Fat16Dir* f = fs_get_file("SHELL   BIN", &spc);
    abstract_t sect = Fat16ClusterToLBA(&spc, f->cluster_lo);

    terminal_color = 0x07;

    uabssmall_t* fa = fs_read_file(f, &spc);
    proalloc((abstract_t)fa);

    init_pit(50);

    asm volatile("sti");

    while (true) {
        render();
        popback();
    }
}