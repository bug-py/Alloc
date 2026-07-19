#include "stack_alloc.h"
#include <stdio.h>
#define DEFAULT_ALIGNEMENT 2*sizeof(void*)

static void* align_ptr_foward(void* ptr,size_t size){
    return (char*)ptr+size-((uintptr_t)ptr%size);
}

void stack_init(stack_t* stack,void* buffer,size_t length){
    stack->buffer=buffer;
    stack->length=length;
    stack->current_offset=0;
    stack->prev_offset=0;
}
void* stack_alloc(stack_t* stack,size_t size){
    void* ptr=stack->buffer+stack->current_offset;
    header_t* header=align_ptr_foward(ptr,8);
    void* new_ptr=align_ptr_foward(header+1,DEFAULT_ALIGNEMENT);
    
    size_t meta_data=(uintptr_t)new_ptr-(uintptr_t)ptr;
    size_t new_offset=stack->current_offset+meta_data+size;
   
    if(new_offset>stack->length) return NULL;

    size_t padding=(uintptr_t)header-(uintptr_t)ptr;
    header->padding=(uint8_t)padding;
    header->prev_offset=stack->prev_offset;
    
    stack->prev_offset=(uintptr_t)stack->buffer-(uintptr_t)new_ptr;
    stack->current_offset=new_offset;
    printf("%p\n",new_ptr);
    return new_ptr;
}
