#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include "tools.h"
#include  <string.h>
#include "malloc.h"
#define LIBFOO_DLL_EXPORTED  __attribute__((visibility("default")))

// var static

extern struct page array[8]; 
// 16 // 32 // 64 // 128 // 512 //

__attribute__((__visibility__("default")))
void *malloc(size_t size)
{
          static int first = 0; 
          size = ALIGN_BSIZE((size)); 
          if (first)
                {
                    if (size > 512)
                              return find_page(size); 
                    else
                              return find_block(size);  
                }
          else
                {
                    first = 1; 
                    create_pages(); 
                    if ( size > 512)
                              return find_page(size); 
                    else
                              return find_block(size); 
                }
}

          /////////// A modifier  trouver solution int for add ////////// 
          //////////////////////////////////////
          ////////////////////////////////////
          ////////////////////////////////
          /////////////////////////////
          ////////////////////////
          ///////////////
          //////////
          //////
          ////
          //
__attribute__((__visibility__("default")))
void free(void *ptr)
{
          if (ptr)
                {
                    printf (":::::::::  FREE  PTR ::::::::::::::: %p ::::::::::: \n", ptr); 
                    int tmp = (int)ptr; 
                    tmp = (tmp<<21)>>21; 
                    pt_page_h head = (pt_page_h) (ptr - tmp); 
                    printf("::::::::::::::: FREE PAGEHEAD : %p :::::::::::::::::::::::: \n", head); 
                    head->full = 0; 
                    head->free = 1; 
                    pt_block block = (pt_block)(ptr - 16); 
                    block->free = 1; 
                }
}

__attribute__((__visibility__("default")))
void *realloc(void *ptr, size_t size)
{
          if (!ptr)
                    return malloc(size); 
          else if (!size && ptr)
                {
                    free(ptr); 
                    return NULL; 
                }
          else
                    return realloc_block(size, ptr);
}
__attribute__((__visibility__("default")))
void *calloc(size_t number, size_t size)
{
          void *ptr = malloc(size); 
          memset(ptr, 0, size); 
          return ptr; 
}
