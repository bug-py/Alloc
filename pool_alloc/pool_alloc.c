#include "pool_alloc.h"

#define DEFAULT_ALIGNEMENT 2*sizeof(void*) 
static size_t align_size_foward(size_t size,size_t align){
    size_t modulo=size%align;
    if(modulo!=0){
        return size+align-modulo;
    }
    return size; 
}
static void* align_ptr_foward(void* ptr,size_t align){
    size_t modulo=(uintptr_t)ptr%align;
    if(modulo!=0){
        return ((char*)ptr)+align-modulo; 
    }
    return ptr;
}
static size_t calc_chunk_size(size_t element_size){
    return align_size_foward(element_size+sizeof(pool_header_t),DEFAULT_ALIGNEMENT);
}
size_t calc_len_for_elem(size_t element_size,size_t chunks){
    return DEFAULT_ALIGNEMENT-1+chunks*calc_chunk_size(element_size);
}

size_t pool_init(pool_t* pool,void* buffer,size_t length,size_t element_size){
    if(calc_len_for_elem(element_size,1)>length) return 0;
    void* align_ptr=align_ptr_foward(buffer,DEFAULT_ALIGNEMENT);
    pool->buffer=align_ptr;
    pool->length_buff=length-((uintptr_t)align_ptr-(uintptr_t)buffer);
    pool->chunk_size=calc_chunk_size(element_size);
    pool->head=NULL;
    pool_free_all(pool);
    return length/pool->chunk_size;
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
    size_t chunks=pool->length_buff/pool->chunk_size;
    pool->head=(pool_header_t*)pool->buffer;
    pool_header_t* prev=NULL;
    pool_header_t* current=pool->head;
    for(size_t i=1;i<chunks;i++){
       prev=current;
       current=(pool_header_t*)((char*)current+pool->chunk_size);
       prev->next=current;
    }
    current->next=NULL;
}