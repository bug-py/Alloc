#include "buddy_alloc.h"
#include <assert.h>
#define HEADER_ALIGNEMENT 8

static inline buddy_block_t* buddy_block_next(buddy_block_t* block){
    return (buddy_block_t*)((char*)block+block->size);
}
static buddy_block_t* buddy_block_split(buddy_block_t* block,size_t size){
    if (size>block->size || !size) return NULL;
    while(block->size>size){
        size_t new_size=block->size/2;
        block->size=new_size;
        buddy_block_t* new_block=buddy_block_next(block);
        new_block->size=new_size;
        new_block->is_free=true;
    }
    return block;
}
static buddy_block_t* find_best_buddy_block(buddy_alloc_t* buddy_alloc,size_t size){
        buddy_block_t* best_block=NULL;
        buddy_block_t* block =buddy_alloc->head;
        while(block<buddy_alloc->tail){
            if( block->is_free && (best_block==NULL || best_block->size>block->size)){
                best_block=block;
            }
            block=buddy_block_next(block);
        }
        return best_block ? buddy_block_split(best_block,size) : NULL;
}
void buddy_alloc_init(buddy_alloc_t* buddy_alloc,void* buffer,size_t size,size_t align){

    align = HEADER_ALIGNEMENT>align ? HEADER_ALIGNEMENT : align;
    
}
void buddy_alloc_free_all(buddy_alloc_t* buddy_alloc){
    buddy_alloc->head->is_free=true;
    buddy_alloc->head->size=(uintptr_t)buddy_alloc->tail-(uintptr_t)buddy_alloc->head;
}