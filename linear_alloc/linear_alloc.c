#include "linear_alloc.h"
#include "align.h"
#include <string.h>

void arena_init(arena_t* arena,void* buffer,size_t length){
    arena->buffer=buffer;
    arena->length=length;
    arena->current_offset=0;
    arena->prev_offset=0;
}
void* arena_alloc_align(arena_t* arena,size_t size,size_t align){
    if(arena==NULL || size==0) return NULL;

    void* ptr=arena->buffer+arena->current_offset;
    void* align_ptr=align_ptr_foward(ptr,align);
    size_t align_offset=(uintptr_t)align_ptr-(uintptr_t)arena->buffer;
    size_t new_offset=align_offset+size;

    if(new_offset>arena->length) return NULL;

    arena->prev_offset=align_offset;
    arena->current_offset=new_offset;

    return align_ptr;
}
inline void* arena_alloc(arena_t* arena,size_t size){
    return arena_alloc_align(arena,size,DEFAULT_ALIGNEMENT);
}
void* arena_realloc(arena_t* arena,void* old_ptr,size_t old_size,size_t new_size){

    if(arena==NULL || new_size==0 ) return NULL;
    if(old_ptr==NULL || old_size==0) return arena_alloc(arena,new_size);

    uintptr_t min=(uintptr_t)arena->buffer;
    uintptr_t max=(uintptr_t)arena->buffer+arena->current_offset;
    if((uintptr_t)old_ptr<min || (uintptr_t)old_ptr>=max) return NULL;

    void* old_alloc_ptr=arena->buffer+arena->prev_offset;

    if(old_ptr==old_alloc_ptr){

        size_t new_offset=arena->prev_offset+new_size;
        if(new_offset>arena->length) return NULL;
        arena->current_offset=new_offset;
        return old_ptr;

    }else{

        void* new_ptr=arena_alloc(arena,new_size);
        if(new_ptr==NULL) return NULL;
        size_t copy_size= new_size>old_size ? old_size : new_size;
        memmove(old_ptr,new_ptr,copy_size);
        return new_ptr;

    }
    
}
void arena_free_all(arena_t* arena){
    if(arena==NULL) return;
    arena->prev_offset=0;
    arena->current_offset=0;
}

void tmp_arena_capture(tmp_arena_t* tmp,arena_t* arena){
    if(arena==NULL || tmp==NULL) return;
    tmp->arena=arena;
    tmp->current_offset=arena->current_offset;
    tmp->prev_offset=arena->prev_offset;
}
void tmp_arena_restore(tmp_arena_t* tmp){
    if(tmp==NULL) return;
    tmp->arena->current_offset=tmp->current_offset;
    tmp->arena->prev_offset=tmp->prev_offset;
}