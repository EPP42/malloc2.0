#ifndef HEADER_H 
#define HEADER_H 

// library
#include <stddef.h>

// define 

#define SIZE_MBLOCK sizeof(struct memory_b)
#define PAGE_SIZE(a) ((a) / 4096)
#define ALIGN_SIZE(a) (((((a)−1)>>2)<<2) + 4)
// structure
struct memory_b
{
          size_t size; // allocated block size 
          int free; // if it's free 
          struct memory_b *next; 
          char limit[1]; 
};

typedef struct memory_b* pt_memory;
// prototypes 

void *malloc(size_t size);
void free(void *ptr); 
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size); 

#endif 
