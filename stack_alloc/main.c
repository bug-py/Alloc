#include <stdio.h>
#include "stack_alloc.h"
int main(){
    stack_t stack;
    char buffer[1000];
    stack_init(&stack,buffer,1000);
    printf("oui\n");
    int* number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    return 0;
}