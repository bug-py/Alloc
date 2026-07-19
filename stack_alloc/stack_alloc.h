#ifndef STACK_H
#define STACK_H
#include <stddef.h>
#include <stdint.h>
typedef struct{
    char* buffer;
    size_t length;
    size_t current_offset;
    size_t prev_offset;
}stack_t;

typedef struct{
    uint8_t padding;
    size_t prev_offset;
}header_t;
void stack_init(stack_t* stack,void* buffer,size_t length);
void* stack_alloc(stack_t* stack,size_t size);
#endif