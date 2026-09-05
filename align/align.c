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
// RETOURNE (HEADER+PADDING)
// | PADDING | | HEADER | | DATA | adresse de DATA alignée sur align 
// adresse de HEADER alignée sur ALIGN si la taille du header est multiple de ALIGN 
// (alignement garantit pour les struct automatique par la compilateur)
// EXEMPLE :
//      HEADER : 16 octets 
//      On veut garantir un alignememnt de 16 octets pour HEADER
//      On veut garantir un alignememnt de 16 octets pour DATA
//      0x01 => |PADDING (15 octets)| |HEADER (16 octets)| |DATA |
//      HEADER => 0x10 "16" aligné sur 16 octets 
//      DATA => 0x20 "32" aligné sur 16 octets
size_t calc_padding_with_header(void* ptr,size_t header_size,size_t align){
    void* align_ptr=align_ptr_foward((char*)ptr+header_size,align);
    return (uintptr_t)align_ptr -(uintptr_t)ptr;
}