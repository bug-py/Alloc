#ifndef POOL_H
#define POOL_H
#include <stddef.h>
#include <stdint.h>
typedef struct pool_header pool_header_t;
struct pool_header{
    struct pool_header* next;
};
typedef struct{
    char* buffer;
    size_t length_buff;
    size_t chunk_size;
    pool_header_t* head;
}pool_t;
size_t pool_init(pool_t* pool,void* buffer,size_t length,size_t element_size);
void* pool_alloc(pool_t* pool);
void pool_free(pool_t* pool,void* ptr);
void pool_free_all(pool_t* pool);
size_t calc_len_for_elem(size_t element_size,size_t chunks);
#endif