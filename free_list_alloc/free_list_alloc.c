#include "free_list_alloc.h"
#include "align.h"
#include <string.h>


int free_list_init(free_list_t* free_list,void* buffer,size_t length,placement_policy_t policy){
    if(length<(DEFAULT_ALIGNEMENT-1)+sizeof(block_header_t)) return -1;
    size_t diff_align=(uintptr_t)align_ptr_foward(buffer,8)-(uintptr_t)buffer;
    buffer=(char*)buffer+diff_align;
    length-=diff_align;
    free_list->buffer=buffer;
    free_list->length_buff=length;
    free_list->policy=policy;
    free_list_free_all(free_list);
    return 0;
}
void free_list_free_all(free_list_t* free_list){
    block_header_t first_block;
    first_block.block_size=free_list->length_buff-sizeof(block_header_t*);
    first_block.free.next=NULL;
    free_list->used=0;
    free_list->head=(block_header_t*)free_list->buffer;
}
