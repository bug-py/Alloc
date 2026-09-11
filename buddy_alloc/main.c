#include "buddy_alloc.h"
#include <stdio.h>
int main(){
    buddy_alloc_t ba;
    char buffer[1024];
    buddy_allocator_init(&ba,buffer,sizeof(buffer),8);
    //void* ptr=buddy_allocator_alloc(&ba,132);
    void* ptr=buddy_allocator_alloc(&ba,496);
    printf("%p\n",ptr);

    void* new_ptr=buddy_allocator_alloc(&ba,286);
    printf("%p\n",new_ptr);


    buddy_allocator_free(&ba,ptr);
    
    new_ptr=buddy_allocator_alloc(&ba,496);
    printf("%p\n",new_ptr);

  
    printf("PAS CRASH");
    return 0;
}
