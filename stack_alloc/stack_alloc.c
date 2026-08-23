#include "stack_alloc.h"
#include "align.h"
#define HEADER_ALIGNEMENT 8
void stack_init(stack_t* stack,void* buffer,size_t length){
    stack->buffer=buffer;
    stack->length=length;
    stack->current_offset=0;
    stack->last_alloc_offset=0;
}
void* stack_alloc_align(stack_t* stack,size_t size,size_t align){
    if (align>256) align=256;
    void* ptr=stack->buffer+stack->current_offset;
    size_t padding=calc_padding_with_header(ptr,sizeof(header_t),align,HEADER_ALIGNEMENT);
    size_t alloc_offset=stack->current_offset+padding;
    size_t new_offset=alloc_offset+size;
    if(new_offset>stack->length) return NULL;

    header_t* header=(header_t*)stack->buffer+alloc_offset-sizeof(header_t);
    header->prev_alloc_offset=stack->last_alloc_offset;
    header->padding=padding;

    stack->last_alloc_offset=alloc_offset;
    stack->current_offset=new_offset;
    void* new_ptr=stack->buffer+alloc_offset;
    return new_ptr;
}
inline void* stack_alloc(stack_t* stack,size_t size){
    return stack_alloc_align(stack,size,DEFAULT_ALIGNEMENT);
}
void* stack_resize(stack_t* stack,void* old_ptr,size_t new_size){
    void* last_alloc_ptr=stack->buffer+stack->last_alloc_offset;
    if(last_alloc_ptr!=old_ptr) return NULL;
    size_t new_offset=stack->last_alloc_offset+new_size;
    if(new_offset>stack->length) return NULL;
    stack->current_offset=new_offset;
    return last_alloc_ptr;
    
}
int stack_free(stack_t* stack,void* old_ptr){
    void* last_alloc_ptr=stack->buffer+stack->last_alloc_offset;
    if(last_alloc_ptr!=old_ptr || old_ptr==stack->buffer) return -1;
    header_t* header= (header_t*)((char*)last_alloc_ptr-sizeof(header_t));
    stack->current_offset=stack->last_alloc_offset-header->padding;
    stack->last_alloc_offset=header->prev_alloc_offset;
    return 0;

}
void stack_free_all(stack_t* stack){
    stack->current_offset=0;
    stack->last_alloc_offset=0;
}