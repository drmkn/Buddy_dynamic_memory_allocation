#ifndef BALLOC_H
#define BALLOC_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define maximum_size 4096
#define minimum_size 32
#define minimum_height 0
#define maximum_height 7
#define meta_size sizeof(BLOCK)
#define MIN(a,b) (a > b) ? b : a

/*Declarations*/
typedef struct block BLOCK;
typedef BLOCK* meta_ptr; 

/*height represents the height of a block in the buddy tree*/
typedef struct block{
    unsigned int height;
    bool is_free;
    meta_ptr prev;
    meta_ptr next;
} BLOCK;


extern unsigned long int compute_size(unsigned height);
extern void freeList_add(meta_ptr block);
extern meta_ptr freeList_pop(int height);
extern meta_ptr freeList_remove(meta_ptr block);
extern meta_ptr find_buddy(meta_ptr block);
extern meta_ptr split_block(meta_ptr block);
extern meta_ptr merge_block(meta_ptr block);
extern int get_height(size_t size);
extern void *bmalloc(size_t size);
extern void bfree(void *ptr);
extern void *bcalloc(size_t nmeb, size_t size);
extern void *brealloc(void *ptr,size_t newsize);
extern void freelist_Status();

#endif