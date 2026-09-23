#include "../lib/abstract.h"

extern unsigned char _heap_start;
extern unsigned char _heap_end;

typedef struct block_meta {
    abstract_t size;
    char free;
    struct block_meta* next;
} block_meta_t;

#define META_SIZE sizeof(block_meta_t)

static void* heap_start_addr = 0;

void init_heap() {
    heap_start_addr = (void*)&_heap_start;
    abstract_t heap_total_size = (abstract_t)(&_heap_end - &_heap_start);

    block_meta_t* initial_block = (block_meta_t*)heap_start_addr;
    initial_block->size = heap_total_size - META_SIZE;
    initial_block->free = 1;
    initial_block->next = 0;
}

void* malloc(abstract_t size) {
    if (size <= 0) return 0;

    if (!heap_start_addr) {
        init_heap();
    }

    block_meta_t* current = (block_meta_t*)heap_start_addr;
    
    while (current) {
        if (current->free && current->size >= size) {
            
            if (current->size > size + META_SIZE) {
                block_meta_t* next_block = (block_meta_t*)((void*)(current + 1) + size);
                
                next_block->size = current->size - size - META_SIZE;
                next_block->free = 1;
                next_block->next = current->next;

                current->size = size;
                current->next = next_block;
            }

            current->free = 0;
            return (void*)(current + 1);
        }
        current = current->next;
    }

    return 0; // No hay memoria suficiente
}