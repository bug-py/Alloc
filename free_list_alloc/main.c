#include <stdio.h>
#include "free_list_alloc.h"
int main(){
    free_list_t fl;
    char buffer[500];
    free_list_init(&fl,buffer+4,sizeof(buffer)-4,Find_First);
    printf("buffer base :%p  length base :%lu\n",buffer+4,sizeof(buffer)-4);
    printf("buffer align :%p length align : %lu\n",fl.buffer,fl.length_buff);
    int* number=free_list_alloc_align(&fl,sizeof(int),1024);
    printf("number_ptr align 1024 %p\n",number);
    printf("PAS CRASH\n");
    return 0;   
}