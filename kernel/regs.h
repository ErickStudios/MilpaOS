#include "../lib/abstract.h"
typedef struct regs {
    abstract_t gs, fs, es, ds;
    abstract_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    abstract_t eip, cs, eflags;
} regs_t;