#include "stack_alloc.h"
#include <string.h>
#define DEFAULT_ALIGNEMENT 2*sizeof(void*)

static void* align_ptr_foward(void* ptr,size_t align){
    size_t modulo=(uintptr_t)ptr%align;
    if(modulo!=0){
        return ((char*)ptr)+align-modulo; 
    }
    return ptr;
}
static size_t align_size_foward(size_t size,size_t align){
    size_t modulo=size%align;
    if(modulo!=0){
        return size+align-modulo;
    }
    return size; 
}
static size_t calc_padding_with_header(void* ptr,size_t size){
    size_t padding=(uintptr_t)align_ptr_foward(ptr,size)-(uintptr_t)ptr;
    size_t needed_space=sizeof(header_t);
    if(needed_space>padding){
        size_t remaining_space=needed_space-padding;
        padding+=align_size_foward(remaining_space,size);
    } 
    return padding;

}
void stack_init(stack_t* stack,void* buffer,size_t length){
    stack->buffer=buffer;
    stack->length=length;
    stack->current_offset=0;
    stack->last_alloc_offset=0;
}
void* stack_alloc(stack_t* stack,size_t size){
    void* ptr=stack->buffer+stack->current_offset;
    size_t padding=calc_padding_with_header(ptr,DEFAULT_ALIGNEMENT);
    size_t alloc_offset=stack->current_offset+padding;
    size_t new_offset=alloc_offset+size;
    if(new_offset>stack->length) return NULL;


    void* header_ptr=stack->buffer+alloc_offset-sizeof(header_t);
    header_t header;

    header.prev_alloc_offset=stack->last_alloc_offset;
    header.padding=padding;
    memcpy(header_ptr,&header,sizeof(header_t));
 

    stack->last_alloc_offset=alloc_offset;
    stack->current_offset=new_offset;
    void* new_ptr=stack->buffer+alloc_offset;
    return new_ptr;
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
    if(last_alloc_ptr!=old_ptr) return -1;
    void* header_ptr=(char*)last_alloc_ptr-sizeof(header_t);
    header_t header;
    memcpy(&header,header_ptr,sizeof(header_t));
    stack->current_offset=stack->last_alloc_offset-header.padding;
    stack->last_alloc_offset=header.prev_alloc_offset;
    return 0;

}
void stack_free_all(stack_t* stack){
    stack->current_offset=0;
    stack->last_alloc_offset=0;
}