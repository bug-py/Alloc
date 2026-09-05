#ifndef BUDDY_H
#define BUDDY_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    size_t size;
    bool is_free;
} buddy_block_t;

typedef struct{
    buddy_block_t* head;
    buddy_block_t* tail;
    size_t align; 
}buddy_alloc_t;
void buddy_alloc_init(buddy_alloc_t* buddy_alloc,void* buffer,size_t size,size_t align);
void buddy_alloc_free_all(buddy_alloc_t* buddy_alloc);
#endif