#include "pool_alloc.h"
#include "align.h"
#define HEADER_ALIGNEMENT 8
static size_t calc_chunk_size(size_t element_size,size_t align){
    return align_size_foward(element_size+sizeof(pool_header_t),align);
}
size_t calc_len_for_elem_align(size_t element_size,size_t chunks,size_t align){
    return align-1+chunks*calc_chunk_size(element_size,align);
}
inline size_t calc_len_for_elem(size_t element_size,size_t chunks){
    return calc_len_for_elem_align(element_size,chunks,DEFAULT_ALIGNEMENT);
}
size_t pool_init_align(pool_t* pool,void* buffer,size_t length,size_t element_size,size_t align){
    if(HEADER_ALIGNEMENT>align) align=HEADER_ALIGNEMENT;
    if(calc_len_for_elem_align(element_size,1,align)>length) return 0;
    void* align_ptr=align_ptr_foward(buffer,align);
    length-=(uintptr_t)align_ptr-(uintptr_t)buffer;
    pool->buffer=align_ptr;
    pool->chunk_size=calc_chunk_size(element_size,align);
    pool->chunk_count=length/pool->chunk_size;
    pool->head=NULL;
    pool_free_all(pool);
    return pool->chunk_count;
}
inline size_t pool_init(pool_t* pool,void* buffer,size_t length,size_t element_size){
    return pool_init_align(pool,buffer,length,element_size,DEFAULT_ALIGNEMENT);
}
void* pool_alloc(pool_t* pool){
    if(pool->head==NULL) return NULL;
    pool_header_t* current_chunk=pool->head;
    void* new_ptr=(current_chunk+1);
    pool->head=pool->head->next;
    return new_ptr;
}
void pool_free(pool_t* pool,void* ptr){
    pool_header_t* current_chunk=(pool_header_t*)((char*)ptr-sizeof(pool_header_t));
    current_chunk->next=pool->head;
    pool->head=current_chunk;
}
void pool_free_all(pool_t* pool){
    pool->head=(pool_header_t*)pool->buffer;
    pool_header_t* prev=NULL;
    pool_header_t* current=pool->head;
    for(size_t i=1;i<pool->chunk_count;i++){
       prev=current;
       current=(pool_header_t*)((char*)current+pool->chunk_size);
       prev->next=current;
    }
    current->next=NULL;
}