#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include "malloc.h"
#define LIBFOO_DLL_EXPORTED  __attribute__((visibility("default")))

        // var static

static struct page array[5]; 


// 16 // 32 // 64 // 128 // 512 //





int size_page_block (int index)
{
          static int size[8] = {8, 16, 32, 64, 128, 256, 512};
          if (0 < index && index < 8)
              return size[index]; 
          else
                    return 1000; 
}

__attribute__((__visibility__("default")))
void *malloc(size_t size)
{
          static int first = 0; 
          printf("ICI"); 
          size = ALIGN_BSIZE((size)); 
          if (first)
                    return find_block(size); 
          else
                {
                    first = 1; 
                    create_pages(); 
                    return find_block(size); 
                }
}


void *find_block(size_t size)
{
                    // printf("size: %ld \n::::::",size); 
          int page = 0; 
          PAGE_NUMBER(size, page) 
          pt_page_h page_head = array[page].page_h; 
          while (page_head->full)
                    page_head = page_head->next_h; 
          printf("::::::::::::::: MALLOC PAGEHEAD : %p :::::::::::::::::::::::: \n", page_head); 
          pt_block block =  page_head->next; 
          while (block->next && !block->free)
                    block = block->next;
          if (!(block->next && block->free))
                {
                    printf("||||||||||||||||||||||||||||   NEWPAGE   |||||||||||||||||||||||||||||||||| \n"); 
                    page_head->full = 1; 
                      return new_page(page_head, page);
                              //     page_head->free = 0; 
                  
                }
          block->free = 0;
           printf (":::::::::  MALLOC   PTR ::::::::::::::: %p ::::::::::: \n", block->limit); 
     return block->limit; 
}


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
                    pt_block block = (pt_block)ptr - 16; 
                    block->free = 1; 
                }
}

void *new_page(pt_page_h head, int page_number)
{
          pt_page_h tmp_s = head; 
          while (tmp_s->next_h)
                    tmp_s = tmp_s->next_h; 
           printf("::::::::::::::: MALLOC PAGEHEAD : %p :::::::::::::::::::::::: \n",tmp_s); 
          tmp_s->next_h = mmap(NULL, PAGE_SIZE, PROT_WRITE 
                               | PROT_READ, MAP_PRIVATE | MAP_ANON, 0, 0); 
          tmp_s = tmp_s->next_h;
          tmp_s->full = 0; 
          tmp_s->head_page = (pt_block)tmp_s; 
          tmp_s->next = (pt_block)(tmp_s->limit + size_page_block(page_number));
          tmp_s->next_h = NULL; 
                    //     tmp_s->free = 1; 
          segment_page(size_page_block(page_number), tmp_s->next); 
          tmp_s->next->free = 0; 
          return tmp_s->next; 
}


void create_pages(void)
{
          for (unsigned int i = 0; i < 5; i++)
                {
                    array[i].page_h = mmap(NULL, PAGE_SIZE, PROT_WRITE 
                                           | PROT_READ, MAP_ANON | MAP_PRIVATE, 0, 0);
                    array[i].size = size_page_block(i); 
                    array[i].page_h->head_page = (pt_block)array[i].page_h; 
                    array[i].page_h->full = 0; 
                              // array[i].page_h->free = 1;
                    array[i].page_h->next = (pt_block)(array[i].page_h->limit + array[i].size);
                    array[i].page_h->next_h = NULL;
                }
          for (unsigned int i = 0; i < 5; i++)
                    segment_page(array[i].size, (array[i].page_h->next));
}

void segment_page(size_t size, pt_block block_add)
{ 
		size_t  counter = 1;  
          pt_block block = block_add; 
                    //printf(":::::::::::::::::::::::::::: %ld ::::::::::::::::::::::::::::::::::\n", size); 
while (counter < NBR_BLOCK(size))
		{
    block->free = 1; 
      block->next = (pt_block)(block->limit + size); 
				  block = block->next; 
				counter++; 
		}
                    //printf("counter : ::::::::::::::::::::::::::%ld::::::::::::::::::::: \n",counter); 
		block->next = NULL; 
}


int indentify_addresse_page(void *addr)
{
		for (int i = 0; i < 7; i++)
				if (addr == array[i].page_h)
						return i; 
		return 0; 
}



/*
void free(void *ptr)
{
          if(ptr)
                {
                    pt_block block = ptr - 16;
                    block->free = 0; 
                    int tmp = (int)ptr; 
                    tmp = (tmp<<20)>>20;
                    pt_page_h head =(void*) ptr - tmp;
                    printf("%p \n",head);
                    int diff = (char*)ptr - (char*)(head->next_b); 
                    if (head->next_b)
                          {
                              if (diff > 0)
                              head->next_b->next = ptr; 
                          }
                    else
                              head->next_b = ptr; 
                }
}
 
 
 
 void *allocate_block(int page_number)
 {
 pt_block block = array[page_number].page_h->next_b; 
 array[page_number].page_h->next_b = array[page_number].page_h->next; 
 if (!array[page_number].page_h->next_b || !array[page_number].page_h->free)
 array[page_number].page_h->full = 1; 
 return block->limit; 
 }

 
 ///// TODO ////////
 
 MACRO that return the size in function of page number 
 match page function to finished 
 MAke my README file 
 Calcul the size of the block 
 
 */  
 
 



/*
void *first_allocation(pt_head page_head)
{
  page_head->full = 1; 
	page_head->next_block
}
*/
/*
	 void *new_page(const int page_number, )
	 {
	 while()
	 array[page_number].p = mmap(NULL, PAGE_SIZE, PROT_WRITE | PROT_READ, MAP_ANON | MAP_PRIVATE, 0, 0);
	 segment_page(size, array[page_number].next_b); 
	 }
 */ 


int free_block(pt_block *last_b)
{
		pt_block block = *last_b; 
		while (block && !block->free)
		{
				*last_b = block; 
				block = block->next;
		}
		if (!block->next && !block->free)
				return NO_MORE_SPACE; 
		if ((*last_b)->next->next)
				return LAST_BLOCK; 
		else
				return FREE_BLOCK;
}


/*
	 void segment_block_ff(pt_block tmp_s)
	 {
	 tmp_s->free = 0; 
	 tmp_s->next =(void*) tmp_s->limit; 

	 }







// crete a new page 

*/
// segment a page 

/*
	 int match_page(size_t size_user)
	 {
	 switch(size_user)
	 {
	 case 16:
	 return 1;
	 case 32: 
	 return 2; 
	 case 64:
	 return 3; 
	 break; 
	 case 128:
	 return 4; 
	 break; 
	 case 256:
	 return 5; 
	 break; 
	 }
	 }



*
int main(void)
{
          int *tab[1000] = {NULL}; 
          for (unsigned int i = 0; i < 1000; i++)
                {
                    tab[i] =  malloc(8); 
                    free(tab[i]); 
                              //  printf("||||| add :%p |||||||||",tab[i]); 
                }
		return 0; 
}
*/

