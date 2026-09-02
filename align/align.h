#ifndef ALIGN_H
#define ALIGN_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#define DEFAULT_ALIGNEMENT sizeof(void*)*2
bool is_power_of_two(size_t x);
void* align_ptr_foward(void* ptr,size_t align);
size_t align_size_foward(size_t size,size_t align);
size_t align_size_backward(size_t size,size_t align);
size_t calc_padding_with_header(void* ptr,size_t header_size,size_t align_header,size_t align_data);
#endif