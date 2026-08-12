#include "align.h"
#include <stdbool.h>
#include <assert.h>
static bool is_power_of_two(size_t x){
    return x && ((x)&(x-1))==0;
}
void* align_ptr_foward(void* ptr,size_t align){
    assert(is_power_of_two(align) && "align is not power of 2");
    size_t modulo=(uintptr_t)ptr%align;
    if(modulo!=0){
        return (char*)ptr+align-modulo;
    }
    return ptr;
}
size_t align_size_foward(size_t size,size_t align){
    assert(is_power_of_two(align) && "align is not power of 2");
    size_t modulo=size%align;
    if(modulo!=0){
        return size+align-modulo;
    }
    return size;
}
//aligne la mémoire du user sur l'alignement la plus grande entre header_size et align_header
// |padding||header| => adresse de fin aligné sur align_header et align_data
// utilisation de la propriété des puissance de 2 
size_t calc_padding_with_header(void* ptr,size_t header_size,size_t align_header,size_t align_data){
    assert(align_size_foward(header_size,align_header)==header_size && "header_size not align");
    size_t align=header_size>align_header ?header_size:align_header;
    size_t align_ptr=align_ptr_foward((char*)ptr+header_size,align);
    return (uintptr_t)ptr -(uintptr_t)align_ptr;
}