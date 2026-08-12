#ifndef ALIGN_H
#define ALIGN_H
#include <stdint.h>
#include <stddef.h>
#define DEFAULT_ALIGNEMENT sizeof(void*)*2
void* align_ptr_foward(void* ptr,size_t align);
size_t align_size_foward(size_t size,size_t align);
#endif