#ifndef MALLOC_H
#define MALLOC_H

// library 
#include <stddef.h>


// Macro

#define ALIGN_BSIZE(size) ((((((size) - 1)>>2)<<2) + 4))
#define PAGE_SIZE  sysconf(_SC_PAGE_SIZE)
#define BLOCK_SIZE  sizeof(struct block_mem)
#define NBR_BLOCK(size) (PAGE_SIZE / ((size)  + BLOCK_SIZE))
#define HEAD_SIZE sizeof(struct head)

#define PAGE_NUMBER(size, page) \
		if (0 < size && size <= 8) page= 0;    \
if (8 < size && size <= 16) page= 1;  \
if (16 < size && page <= 32) page = 2;   \
if (32 < size && size <= 64) page = 3;    \
if (64 <size && size <= 128) page = 4;   \
if (128 < size && size <= 256) page = 5;\
if (256 < size && size <= 512)  page = 6; \
if (size > 512) page = 7; 


#define INDEX_ARRAY(size, page)\



// Macro 
#define NO_MORE_SPACE 3 
#define LAST_BLOCK 1
#define FREE_BLOCK 0 // block in the middle 
// typedef 

typedef  struct block_mem* pt_block; 
typedef struct  head* pt_page_h;

// structure
struct page
{
		long size; 
		pt_page_h page_h;  
};


struct block_mem
{
		int free; 
  pt_page_h head; 
		pt_block next;
		char limit[1]; 
}; 

struct head
{
		int full;// 
  size_t size;  
  int free; 
		pt_block head_page;// save the page head add// need able to free  
		pt_block next; 
		pt_page_h next_h; // save the address head next block 
		char limit[1]; 
}; 
void free(void *ptr); 
void *malloc(size_t size);
void *calloc(size_t number, size_t size); 
void *realloc(void *ptr, size_t size); 

#endif 
