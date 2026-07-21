#ifndef STACK_H
#define STACK_H
#include <stddef.h>
#include <stdint.h>
typedef struct{
    char* buffer;
    size_t length;
    size_t current_offset;
    size_t last_alloc_offset;
}stack_t;

typedef struct{
    uint8_t padding;
    size_t prev_alloc_offset;
}header_t;
void stack_init(stack_t* stack,void* buffer,size_t length);
void* stack_alloc(stack_t* stack,size_t size);
void* stack_resize(stack_t* stack,void* old_ptr,size_t new_size);
int stack_free(stack_t* stack,void* old_ptr);

#endif