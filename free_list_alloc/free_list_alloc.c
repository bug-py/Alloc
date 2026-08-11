#include "free_list_alloc.h"
#include <string.h>
#define DEFAULT_ALIGNEMENT sizeof(void*)*2
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
    size_t needed_space=sizeof(block_header_t);
    if(needed_space>padding){
        size_t remaining_space=needed_space-padding;
        padding+=align_size_foward(remaining_space,size);
    } 
    return padding;

}
static void read_header(block_header_t* current,block_header_t* copy){
    memcpy(copy,current,sizeof(block_header_t));
}
static void write_header(block_header_t* current,block_header_t* src){
    memcpy(current,src,sizeof(block_header_t));
}


int free_list_init(free_list_t* free_list,char* buffer,size_t length,placement_policy_t policy){
    free_list->buffer=buffer;
    free_list->length_buff=length;
    free_list->policy=policy;
    if(length<sizeof(block_header_t)) return -1;
    free_list_free_all(free_list);
    return 0;
}
void free_list_free_all(free_list_t* free_list){
    block_header_t first_block;
    first_block.block_size=free_list->length_buff-sizeof(block_header_t*);
    first_block.free.next=NULL;
    write_header((block_header_t*)free_list->buffer,&first_block);
    free_list->used=0;
    free_list->head=(block_header_t*)free_list->buffer;
}
