#include "align.h"
#include <assert.h>
bool is_power_of_two(size_t x){
    return x && (x &(x-1))==0;
}
static size_t propagate_hight_bit(size_t x){
    x |=x >>1;
    x |=x >>2;
    x |=x >>4;
    x |=x >>8;
    x |=x >> 16;
    x |=x >> 32;
    return x;
}
size_t find_last_power_of_two(size_t x){
    x = propagate_hight_bit(x);
    return x ^ (x>>1);
}
size_t find_next_power_of_two(size_t x){
    if(is_power_of_two(x)) return x;
    return propagate_hight_bit(x)+1;
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
size_t align_size_backward(size_t size,size_t align){
    assert(is_power_of_two(align) && "align is not power of 2");
    size_t modulo=size%align;
    if(modulo!=0){
        return size-modulo;
    }
    return size;
}
//aligne la mémoire du user sur l'alignement la plus grande entre header_size et align_header
// |padding||header| => adresse de fin aligné sur align_header et align_data
// utilisation de la propriété des puissance de 2 
size_t calc_padding_with_header(void* ptr,size_t header_size,size_t align_header,size_t align_data){
    assert(align_size_foward(header_size,align_header)==header_size && "header_size not align");
    size_t align=(align_header>align_data) ?align_header:align_data;
    void* align_ptr=align_ptr_foward((char*)ptr+header_size,align);
    return (uintptr_t)align_ptr -(uintptr_t)ptr;
}