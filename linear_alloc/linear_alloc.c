#include "linear_alloc.h"
#define DEFAULT_ALIGNEMENT 2*sizeof(void*) // un alignement de 16 bit n'est pas toujours requis c'est une convention


static void* align_ptr_foward(void* ptr,size_t size){
    return ((char*)ptr)+(size)-((uintptr_t)ptr % size); // j'ajoute au ptr le nombre d'octet pour atteindre la prochaine valeur alignée
}
void arena_init(arena_t* arena,void* buffer,size_t length){
    arena->buffer=buffer;
    arena->length=length;
    arena->current_offset=0;
    arena->prev_offset=0;
}
int arena_alloc(arena_t* arena,size_t size){
    void* align_ptr=align_ptr_foward((arena->buffer+arena->current_offset),DEFAULT_ALIGNEMENT);
    return -1;
}