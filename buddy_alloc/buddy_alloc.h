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
#endif