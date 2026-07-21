#include "stack_alloc.h"
#include <string.h>
#define DEFAULT_ALIGNEMENT 2*sizeof(void*)

static void* align_ptr_foward(void* ptr,size_t size){
    return (char*)ptr+size-((uintptr_t)ptr%size);
}

static size_t calc_padding_with_header(void* ptr,size_t size){
    size_t padding=(uintptr_t)align_ptr_foward(ptr,size)-(uintptr_t)ptr;
    size_t needed_space=sizeof(header_t);
    if(needed_space>padding){
        size_t remaining_space=needed_space-padding;
        padding+=remaining_space+size-(remaining_space%size);
    } 
    return padding;

}
void stack_init(stack_t* stack,void* buffer,size_t length){
    stack->buffer=buffer;
    stack->length=length;
    stack->current_offset=0;
    stack->prev_offset=0;
}
void* stack_alloc(stack_t* stack,size_t size){
    void* ptr=stack->buffer+stack->current_offset;
    size_t padding=calc_padding_with_header(ptr,DEFAULT_ALIGNEMENT);
    size_t alloc_offset=stack->current_offset+padding;
    size_t new_offset=alloc_offset+size;
    if(new_offset>stack->length) return NULL;


    void* header_ptr=stack->buffer+alloc_offset-sizeof(header_t);
    header_t header;

    header.prev_offset=stack->prev_offset;
    header.padding=padding;
    header.alloc_size=size;
    memcpy(header_ptr,&header,sizeof(header_t));
 

    stack->prev_offset=alloc_offset;
    stack->current_offset=new_offset;
    void* new_ptr=stack->buffer+alloc_offset;
    return new_ptr;
}
int stack_free(stack_t* stack,void* old_ptr){
    void* ptr=stack->buffer+stack->prev_offset;
    if(ptr!=old_ptr) return -1;
    void* header_ptr=(char*)ptr-sizeof(header_t);
    header_t header;
    memcpy(&header,header_ptr,sizeof(header_t));
    stack->current_offset=stack->prev_offset-header.padding;
    stack->prev_offset=header.prev_offset;
    return 0;

}