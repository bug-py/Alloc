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
typedef struct{
    arena_t* arena;
    size_t current_offset;
    size_t prev_offset;
}tmp_arena_t;

void arena_init(arena_t* arena,void* buffer,size_t length);
void* arena_alloc(arena_t* arena,size_t size);
void* arena_realloc(arena_t* arena,void* old_ptr,size_t old_size,size_t new_size);
void arena_free_all(arena_t* arena);

void tmp_arena_capture(tmp_arena_t* tmp,arena_t* arena);
void tmp_arena_restore(tmp_arena_t* tmp);
#endif