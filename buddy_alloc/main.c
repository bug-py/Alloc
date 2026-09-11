#include "buddy_alloc.h"
#include <stdio.h>
int main(){
    buddy_alloc_t ba;
    char buffer[1024];
    buddy_allocator_init(&ba,buffer,sizeof(buffer),16);
    printf("PAS CRASH");
    return 0;
}
