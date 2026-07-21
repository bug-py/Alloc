#include <stdio.h>
#include "stack_alloc.h"
int main(){
    stack_t stack;
    char buffer[1000];
    stack_init(&stack,buffer,300);
    int* number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    number=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *number=45;
    int* p;
    p=stack_alloc(&stack,sizeof(int));
    printf("%p\n",p);
    printf("%i\n",stack_free(&stack,number));
    p=stack_alloc(&stack,sizeof(int));
    printf("%p\n",number);
    *p=45;
    return 0;
}