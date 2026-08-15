#include "free_list_alloc.h"
#include "align.h"
#include <assert.h>
static void remove_node(block_header_t** head_ptr,block_header_t* prev_node,block_header_t* del_node){
    assert(del_node!=NULL);
    if(prev_node==NULL){
        assert(head_ptr!=NULL);
        *head_ptr=del_node->free.next;
    }else{
        prev_node->free.next=del_node->free.next;
    }
}
static void insert_node(block_header_t** head_ptr,block_header_t* prev_node,block_header_t* new_node){
    assert(new_node!=NULL);
    if(prev_node==NULL){
        assert(head_ptr!=NULL);
        new_node->free.next=*head_ptr;
        *head_ptr=new_node;
    }else{
        new_node->free.next=prev_node->free.next;
        prev_node->free.next=new_node;
        
    }

}
static block_header_t* find_first_node(block_header_t* head,size_t size,size_t align,size_t* padding_ptr,block_header_t** prev_node_ptr){
    block_header_t* current=head;
    block_header_t* prev_node=NULL;
    size_t padding=0;
    while(current!=NULL){
        padding=calc_padding_with_header(current,sizeof(block_header_t),HEADER_ALIGNEMENT,align);
        size_t require_space=size+padding;
        if(current->block_size>=require_space){
            break;
        }
        prev_node=current;
        current=current->free.next;
    }
    if(padding_ptr) (*padding_ptr)=padding;
    if(prev_node_ptr) (*prev_node_ptr)=prev_node;
    return current;
}
static block_header_t* find_best_node(block_header_t* head,size_t size,size_t align,size_t* padding_ptr,block_header_t** prev_node_ptr){
    block_header_t* best_node=NULL;
    block_header_t* prev_best_node=NULL;
    size_t padding_best_node=0;

    block_header_t* current=head;
    block_header_t* prev_node=NULL;
    
    size_t smallest_diff=~(size_t)0;
    while(current!=NULL){
        size_t padding=calc_padding_with_header(current,sizeof(block_header_t),HEADER_ALIGNEMENT,align);
        size_t require_space=size+padding;
        if(current->block_size>=require_space){
            size_t diff=current->block_size-require_space;
            if(diff<smallest_diff){
                best_node=current;      
                prev_best_node=prev_node;          
                padding_best_node=padding;
                smallest_diff=diff;
            }
            if(diff==0) break;
        }
        prev_node=current;
        current=current->free.next;
    }
    if(padding_ptr) (*padding_ptr)=padding_best_node;
    if(prev_node_ptr) (*prev_node_ptr)=prev_best_node;
    return best_node;
}

int free_list_init(free_list_t* free_list,void* buffer,size_t length,placement_policy_t policy){
    assert(length>(HEADER_ALIGNEMENT-1)+sizeof(block_header_t) && "no space");
    assert(policy==Find_Best || policy==Find_First );
    size_t diff_align=(uintptr_t)align_ptr_foward(buffer,HEADER_ALIGNEMENT)-(uintptr_t)buffer; 
    free_list->buffer=(char*)buffer+diff_align;
    free_list->length_buff=align_size_backward(length-diff_align,HEADER_ALIGNEMENT);
    free_list->policy=policy;
    free_list_free_all(free_list);
    return 0;
}
void* free_list_alloc_align(free_list_t* free_list,size_t size,size_t align){
    size=align_size_foward(size,HEADER_ALIGNEMENT);
    block_header_t* free_block;
    block_header_t* prev_block;
    size_t padding;
    
    if(free_list->policy==Find_First){
        free_block=find_first_node(free_list->head,size,align,&padding,&prev_block);
    }else if(free_list->policy==Find_Best){
        free_block=find_best_node(free_list->head,size,align,&padding,&prev_block);
    }
    if(free_block==NULL) return NULL;

    size_t size_block_alloc=size+padding;
    size_t remaining=free_block->block_size-size_block_alloc;
    if(remaining>sizeof(block_header_t)){
        block_header_t* new_node=(block_header_t*)((char*)free_block+size_block_alloc);
        new_node->block_size=remaining;
        insert_node(&(free_list->head),free_block,new_node);
    }else{
        size_block_alloc+=remaining;
    }
    remove_node(&(free_list->head),prev_block,free_block);


    size_t align_padding=padding-sizeof(block_header_t);
    block_header_t* alloc_block=(block_header_t*)((char*)free_block+align_padding);
    alloc_block->block_size=size_block_alloc;
    alloc_block->alloc.padding=align_padding;
    free_list->used+=size_block_alloc;

    return (char*)alloc_block+sizeof(block_header_t);
}
inline void* free_list_alloc(free_list_t* free_list,size_t size){
    return free_list_alloc_align(free_list,size,DEFAULT_ALIGNEMENT);
}
void free_list_free(free_list_t* free_list,void* ptr){
    assert((uintptr_t)ptr%HEADER_ALIGNEMENT==0 && "not ptr valid");
    block_header_t* alloc_block=(block_header_t*)((char*)ptr-sizeof(block_header_t));
    size_t block_size=alloc_block->block_size;
    size_t padding=alloc_block->alloc.padding;

    block_header_t* free_block=(block_header_t*)((char*)alloc_block-padding);
    free_block->block_size=block_size;
    free_block->free.next=NULL;
    block_header_t* current=free_list->head;
    block_header_t* prev_node=NULL;
    while(current!=NULL){
        if(current>free_block){
            break;
        }
        prev_node=current;
        current=current->free.next;
    }
    insert_node(&(free_list->head),prev_node,free_block);
    free_list->used-=block_size;

}
void free_list_free_all(free_list_t* free_list){
    block_header_t* first_block=(block_header_t*)free_list->buffer;
    first_block->block_size=free_list->length_buff;
    first_block->free.next=NULL;
    free_list->head=first_block;
    free_list->used=0;
}
