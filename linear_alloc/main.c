#include "linear_alloc.h"
#include <stdio.h>
int main(){
    char buffer[1000];
    arena_t arena;
    arena_init(&arena,buffer,1000);
    void* ptr;
    ptr=arena_alloc(&arena,100);
    printf("%p\n",ptr);
    ptr=arena_alloc(&arena,200);
    printf("%p\n",ptr);
    ptr=arena_alloc(&arena,500);
    printf("%p\n",ptr);
    ptr=arena_alloc(&arena,500);
    printf("%p\n",ptr);
}