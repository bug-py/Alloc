#include <stdio.h>
#include "free_list_alloc.h"
int main(){
    free_list_t fl;
    char buffer[250];
    free_list_init(&fl,buffer,sizeof(buffer),Find_First);
    printf("buffer base :%p  length base :%lu\n",buffer,sizeof(buffer));
    printf("buffer align :%p length align : %lu\n",fl.buffer,fl.length_buff);
    int* number=free_list_alloc_align(&fl,sizeof(int),16);
    printf("number_ptr align 16 %p\n",number);
    number=free_list_alloc_align(&fl,sizeof(int),16);
    free_list_free(&fl,number);
    printf("number_ptr align 16 %p\n",number);
    number=free_list_alloc_align(&fl,sizeof(int),16);
    printf("number_ptr align 16 %p\n",number);
    number=free_list_alloc_align(&fl,sizeof(int),16);
    free_list_free(&fl,number);
    printf("number_ptr align 16 %p\n",number);
    number=free_list_alloc_align(&fl,sizeof(int),16);
    printf("number_ptr align 16 %p\n",number);
    number=free_list_alloc_align(&fl,sizeof(int),16);
    free_list_free(&fl,number);
    printf("number_ptr align 16 %p\n",number);
    printf("used octet : %lu\n",fl.used);
    printf("PAS CRASH\n");
    return 0;   
}