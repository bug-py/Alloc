#include <stdio.h>
#include "stack_alloc.h"
int main(){
    stack_t stack;
    char buffer[1000];
    stack_init(&stack,buffer,1000);
    int *number , *ptr ;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    ptr=stack_alloc(&stack,sizeof(int)*10);
    printf("%p\n",ptr);
    ptr[0]=55;
    ptr[1]=-68;
    ptr[4]=77;
    ptr=stack_resize(&stack,ptr,900);
    printf("%p\n",ptr);
    return 0;
}