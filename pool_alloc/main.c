#include <stdio.h>
#include "pool_alloc.h"
typedef struct {
    int age;
    double attack;
    double defense;
    double speed;
    char name;
}player_t;
int main(){
    pool_t pool;
    size_t len=calc_len_for_elem(sizeof(player_t),2);
    char buffer[len];
    size_t chunks=pool_init(&pool,buffer,len,sizeof(player_t));
    printf("size : %lu\n",sizeof(player_t));
    printf("len :%lu\n",len);
    printf("count : %lu\n",chunks);
    printf("=========== \n");
    player_t* tom;
    player_t* fatima;
    player_t* eric;
    tom=pool_alloc(&pool);
    tom->age=45;
    pool_free(&pool,tom);
    tom=pool_alloc(&pool);
    printf("age : %i\n",tom->age);
    fatima=pool_alloc(&pool);
    fatima->name='F';
    fatima->age=100;
    printf("%p\n",tom);
    printf("%p\n",fatima);    
    eric=pool_alloc(&pool);
    printf("%p\n",eric);
}