#include "../lib/mem.h"

struct idt_entry {
    abslittl_t  base_low;
    abslittl_t  sel;
    uabssmall_t always0;
    uabssmall_t flags;
    abslittl_t  base_high;
} __attribute__((packed));

struct idt_ptr {
    abslittl_t limit;
    abstract_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr   idtp;

void idt_set_gate(uabssmall_t num, abstract_t base, abslittl_t sel, uabssmall_t flags) {
    idt[num].base_low  = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags; 
}

void init_idt() {
    memset(&idt, 0, sizeof(idt));
}

void idt_install() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (abstract_t)&idt;

    asm volatile("lidt %0" :: "m"(idtp));
}