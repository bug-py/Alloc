#include "linear_alloc.h"
#include <stdio.h>
#include <string.h>
typedef struct {
    int age;
    char lettre;
    double water;
}data_t;
void alloc(arena_t* arena,char** name_arg,int** number_arg,data_t** data_arg){
    char* name=arena_alloc(arena,sizeof(char)*7);
    if(name==NULL) printf("NO SPACE\n");
    strcpy(name,"ROBERT");
    
    *name_arg=name;

    int* number=arena_alloc(arena,sizeof(int));
    if(number==NULL) printf("NO SPACE\n");
    *number=400;

    *number_arg=number;

    data_t* data=arena_alloc(arena,sizeof(data_t));
    if(data==NULL) printf("NO SPACE\n");
    data->age=45;
    data->lettre='c';
    data->water=-2.5;

    *data_arg=data;

}

void print(char* name,int* number,data_t* data){
    printf("\nname : %s\n",name);
    printf("number : %i\n",*number);
    printf("data :\n age : %i \n lettre : %c \n water : %f\n",data->age,data->lettre,data->water);

}
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
    
    ptr=arena_realloc(&arena,ptr,500,600);
    printf("%p\n",ptr);
    ptr=arena_realloc(&arena,ptr,600,100);
    printf("%p\n",ptr);
    arena_free_all(&arena);
    
    char* name;
    int* number;
    data_t* data;
    alloc(&arena,&name,&number,&data);

    print(name,number,data);

    tmp_arena_t tmp;
    tmp_arena_capture(&arena,&tmp);
    arena_free_all(&arena);
    tmp_arena_restore(&tmp);

    print(name,number,data);
}

