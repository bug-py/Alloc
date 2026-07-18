#ifndef LINEAR_H
#define LINEAR_H
#include <stddef.h>
#include <stdint.h>
typedef struct{
    char * buffer;
    size_t length;
    size_t current_offset;
    size_t prev_offset; 
}arena_t;

void arena_init(arena_t* arena,void* buffer,size_t length);
int arena_alloc(arena_t* arena,size_t size);
#endif