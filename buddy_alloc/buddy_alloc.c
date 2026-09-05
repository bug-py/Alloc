#include "buddy_alloc.h"
#include "align.h"
#include <assert.h>
#define HEADER_ALIGNEMENT alignof(buddy_block_t)

static inline buddy_block_t* buddy_block_next(buddy_block_t* block){
    return (buddy_block_t*)((char*)block+block->size);
}
static buddy_block_t* buddy_block_split(buddy_block_t* block,size_t size){
    if (size>block->size || !size) return NULL;
    while(block->size>size){
        size_t new_size=block->size/2;
        block->size=new_size;
        block=buddy_block_next(block);
        block->size=new_size;
        block->is_free=true;
    }
    return block;
}
static buddy_block_t* find_best_buddy_block(buddy_alloc_t* buddy_alloc,size_t size){
        buddy_block_t* best_block=NULL;
        buddy_block_t* block =buddy_alloc->head;
        while(block<buddy_alloc->tail){
            if( block->is_free && block->size>=size && (best_block==NULL || best_block->size>=block->size)){
                best_block=block;
            }
            block=buddy_block_next(block);
        }
        return best_block ? buddy_block_split(best_block,size) : NULL;
}
void buddy_alloc_init(buddy_alloc_t* buddy_alloc,void* buffer,size_t size,size_t align){
    if(HEADER_ALIGNEMENT>align) align=HEADER_ALIGNEMENT;

    assert(size>align-1 || "NO ENOUGH SPACE FOR ALIGN");
    size_t padding_align=calc_padding_with_header(buffer,sizeof(buddy_block_t),align)-sizeof(buddy_block_t);
    size_t size_first_block=find_last_power_of_two(size-padding_align);
    size_t min_block_size=find_next_power_of_two(align_size_foward(sizeof(buddy_block_t)+1,align));
    assert(size_first_block>=min_block_size || "NO SPACE");
    buddy_alloc->head=(buddy_block_t*)((char*)buffer+padding_align);
    buddy_alloc->tail=(buddy_block_t*)((char*)buddy_alloc->head+size_first_block);
    buddy_alloc->min_block_size=min_block_size;
    buddy_alloc_free_all(buddy_alloc);
    
}
void buddy_alloc_free_all(buddy_alloc_t* buddy_alloc){
    buddy_alloc->head->is_free=true;
    buddy_alloc->head->size=(uintptr_t)buddy_alloc->tail-(uintptr_t)buddy_alloc->head;
}