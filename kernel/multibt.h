#include "../lib/abstract.h"
#include <stdint.h>

typedef struct {
    abstract_t flags;

    abstract_t mem_lower;
    abstract_t mem_upper;

    abstract_t boot_device;
    abstract_t cmdline;

    abstract_t mods_count;
    abstract_t mods_addr;

    abstract_t  syms[4];

    abstract_t mmap_length;
    abstract_t mmap_addr;

    abstract_t drives_length;
    abstract_t drives_addr;

    abstract_t config_table;
    abstract_t boot_loader_name;
    abstract_t apm_table;

    abstract_t vbe_control_info;
    abstract_t vbe_mode_info;
    abslittl_t vbe_mode;
    abslittl_t vbe_interface_seg;
    abslittl_t vbe_interface_off;
    abslittl_t vbe_interface_len;

    uint64_t framebuffer_addr;
    abstract_t framebuffer_pitch;
    abstract_t framebuffer_width;
    abstract_t framebuffer_height;
    uabssmall_t  framebuffer_bpp;
    uabssmall_t  framebuffer_type;
    abslittl_t reserved;
} multiboot_info_t;

multiboot_info_t *globInf;