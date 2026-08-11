#ifndef FREE_LIST_H
#define FREE_LIST_H
#include <stddef.h>
#include <stdint.h>
typedef struct block_header block_header_t;
struct block_header {
    size_t block_size;
    union {
        struct {
            struct block_header* next;
        }free;
        struct{
            size_t padding;
        }alloc;
    };
};
typedef enum{
    Find_First,
    Find_Best
}placement_policy_t;

typedef struct{
    char* buffer;
    size_t length_buff;
    size_t used;
    placement_policy_t policy;
    block_header_t* head;
}free_list_t;

int free_list_init(free_list_t* free_list,char* buffer,size_t length,placement_policy_t polity);
void free_list_free_all(free_list_t* free_list);
#endif