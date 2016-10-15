#ifndef TOOLS_H 
#define TOOLS_H 

#include <stddef.h>
#include "malloc.h"

void segment_page(size_t size, pt_block block_add); 
long size_page_block (int index); 
void create_pages(void); 
void *allocate_block(int page_number); 
void *find_block(size_t size); 
void *find_page(size_t size);
void *new_page(pt_page_h head, int page_number); 
void *realloc_block(size_t size, void *ptr); 
void *handle_page(pt_page_h head, void *ptr, size_t size); 
void *realloc_block(size_t size, void *ptr); 
#endif 