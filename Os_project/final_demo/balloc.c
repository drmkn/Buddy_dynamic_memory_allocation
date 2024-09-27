#include "balloc.h"

/*heightwise freelist*/
meta_ptr freeList[maximum_height-minimum_height+1] = {NULL};//to include root lvl we add 1

/*Given the height of a block it computes the size of it*/
unsigned long int compute_size(unsigned height){ 
    unsigned long int size = minimum_size;
    int i = maximum_height;
    while (i > height){
        size <<= 1;
        i--;
    }
    return size;
}

/*Add a block in the freelist*/
void freeList_add(meta_ptr block){
    int height = block->height;
    block->is_free = 1;
    if (NULL == freeList[height]){ //for first time inserting into free_list(initializing portion)
        freeList[height] = block;
        freeList[height]->next = NULL;
        freeList[height]->prev = NULL;
        return;
    }
    freeList[height]->prev = block; //entering through head 
    block->next = freeList[height];
    freeList[height] = block;
    return;
}

/*Given a height, it removes the head of the freelist of that height and returns it to us*/
meta_ptr freeList_pop(int height){ //poping through head
    meta_ptr block = freeList[height];
    if (NULL == block->next){ //when list contains only one element
        freeList[height] = NULL;
        block->is_free = 0; //as this block needed in use(obviously) for that reason is_free = 0
        return block;
    }
    meta_ptr next_block = block->next; //removing from head procedure
    next_block->prev = NULL;
    block->next = NULL;
    block->is_free = 0;
    freeList[height] = next_block;
    return block;
}

/*Removes a particular block from the free list*/
meta_ptr freeList_remove(meta_ptr block){
    assert(block->is_free == 1);
    int height = block->height;
    meta_ptr node = freeList[height];
    while (node != block){
        node = node->next;
    }
    if (NULL == node->prev){ // head prev is null,poping from list
        return freeList_pop(height);
    }
    else if (NULL == node->next){//tail element
        meta_ptr prev_node = node->prev;
        node->prev = NULL;
        prev_node->next = NULL;
        node->is_free=0; 
        return node;
    }
    else{//any intermediate node
        meta_ptr prev_node = node->prev;
        meta_ptr next_node = node->next;
        prev_node->next = next_node;next_node->prev = prev_node;
        node->next = node->prev = NULL;node->is_free=0;
        return node;
    }
}

/*Finds the buddy of a given block*/
meta_ptr find_buddy(meta_ptr block){
    //meta_ptr buddy = (meta_ptr)((long int)block ^ (long int)compute_size(block->height));
    
    //printf("%p\n",(meta_ptr)((long int)block ^ (long int)compute_size(block->height)));
    return (meta_ptr)((long int)block ^ (long int)compute_size(block->height));
}

/*Splits a given block*/
meta_ptr split_block(meta_ptr block){
    assert(block->is_free == 1);
    meta_ptr primary,buddy;
    int curr_height = block->height;
    primary = freeList_remove(block);
    primary->height = curr_height+1;primary->is_free=1;
    buddy = find_buddy(primary);
    buddy->height = curr_height+1;buddy->is_free = 1;
    freeList_add(buddy);freeList_add(primary);
    return primary;
}

/*Merges a given block with its buddy if it is free*/
meta_ptr merge_block(meta_ptr block){
    assert(block->is_free == 1);
    meta_ptr block1 = block;
    meta_ptr block2 = find_buddy(block);
    meta_ptr primary,buddy;
    if ((long int)block1 < (long int)block2){//typecasting to know which one is actuall left buddy(primary) and right buddy
        primary = block1;buddy = block2;
    }
    else{
        primary = block2;buddy = block1;
    }
    if (primary->is_free == 1 && buddy->is_free == 1){
        int height = primary->height;
        freeList_remove(primary);freeList_remove(buddy);
        primary->height = height-1;
        freeList_add(primary);
        return primary;}
    
    //freeList_remove(block1);
    return block1;        
}

/*Returns the height of the block required for a given size request*/
int get_height(size_t size){
    assert(size + meta_size <= maximum_size);//3
    int height = minimum_height;
    size_t allocated = maximum_size;
    size_t total = size + meta_size;
    while (total < allocated){
        allocated >>= 1;
        height += 1;
    }
    return height-1;
}

/*Buddy memory allocator*/
void *bmalloc(size_t size){
    int height = get_height(size);
    int temp = height;
    meta_ptr alloc;
    while (NULL == freeList[temp] && temp >= 0){
        temp--;
    }

    if (temp == -1){ //root case handeling,all freelist are empty fully 
        alloc = (meta_ptr)sbrk(maximum_size);//root meta_ptr
        if ((void *)-1 == (void *)alloc) return NULL;//sbrk failed to allocate 
        alloc->height = minimum_height;alloc->is_free = 1;
        freeList_add(alloc);temp = 0;
    }
    else{
        alloc = freeList[temp];
        assert(alloc != NULL);
    }

    while (temp < height){
        alloc = split_block(alloc);
        temp++;
    }
    alloc = freeList_remove(alloc);
    return (void *)((char *)alloc + meta_size); //Seems to work
}

/*free for buddy memeory allocator*/
void bfree(void *ptr){
    meta_ptr block = (meta_ptr)((char *)ptr-meta_size);
    freeList_add(block);
    meta_ptr buddy;
    while((block->height > 0) && (((buddy = find_buddy(block))->is_free) == 1) && buddy->height == block->height){
        block = merge_block(block);
    }
    return;
}

void *bcalloc(size_t nmeb, size_t size){
    void *ptr = bmalloc(nmeb*size);
    for (int i = 0;i<nmeb*size;i++){
        *((char *)ptr + i) = 0 ;//byte by byte zero allocation
    }

    return ptr;
}

void *brealloc(void *ptr,size_t newsize){
    assert(newsize < maximum_size-meta_size);
    void *newptr;
    if (NULL == ptr){//works as just malloc
        return bmalloc(newsize);
    }
    if (ptr && newsize == 0){//works just as free
        bfree(ptr);
        return ptr;
    }
    meta_ptr block = (meta_ptr)((char *)ptr - meta_size);
    size_t oldsize = compute_size(block->height);
    int newheight = get_height(newsize);
    if (newheight == block->height){
        return ptr;
    }

    else if(newheight > block->height){//splitting case
        int temp = newheight-block->height;
        freeList_add(block);
        while (temp != 0){
            block = split_block(block);
            temp--;
        }
        block = freeList_remove(block); //here we do is_free = 0
        return ptr;
    }

    else if(newheight < block->height){
        int temp = block->height - newheight;
        freeList_add(block);
        while (temp != 0){// block safegaurd for the case if merging(else case of merge) fails
            int oldh = block->height;
            block = merge_block(block);
            if (oldh == block->height) break;
            temp--;
        }
        freeList_remove(block);
        newptr = ((char *)block + meta_size);
        if (temp == 0){
            if (newptr == ptr) return ptr; //no need of copying
            for (size_t i = 0;i<oldsize;i++){
            *((char *)newptr + i) = *((char *)ptr + i);
            }
        }
        else {// temp != 0 merge was not succesfull fully
            newptr = bmalloc(newsize);
            for (size_t i = 0;i<oldsize;i++){
                *((char *)newptr + i) = *((char *)ptr + i);
            }
            bfree((char *)block+meta_size);
        }
    }
    return newptr;
}



void freelist_Status(){
    meta_ptr node;
    for(int i = 0;i<maximum_height+1;i++){
        printf("freelist[height->%d]-->",i);
        int j=1;
        node = freeList[i];
        while(node){
            printf("[%d] addr->%p  is_free->%d  size->%ld\t",j,node,node->is_free,compute_size(node->height));
            node = node->next;
            j++;
        }
        printf("\n");
    }
    printf("end\n\n");
    return;
}
