#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "malloc.h"
#include <string.h>
#include "tools.h"
#define LIBFOO_DLL_EXPORTED  __attribute__((visibility("default")))

struct page array[8]; 
int first[7] = {0}; 
long size_page_block (int index)
{
		static int size[8] = {8, 16, 32, 64, 128, 256, 512};
		if (0 <= index && index < 8)
				return size[index]; 
		else
				return PAGE_SIZE; 
}

          // allocate a page for the function find new block 
void *allocate_page (size_t size)
{
		static int first = 2; 
		if (first == 2)
		{
				first = 3; 
				if ((array[7].page_h = mmap(NULL, size + HEAD_SIZE, PROT_WRITE 
                             | PROT_READ, MAP_ANON | MAP_PRIVATE, 0, 0)) == (void*)-1)
      return NULL; 
				array[7].page_h->head_page = (pt_block)array[7].page_h; 
				array[7].page_h->next_h = NULL; 
				array[7].page_h->next = (pt_block)array[7].page_h->limit; 
				array[7].page_h->next->free = 0; 
    array[7].page_h->next->head = array[7].page_h;  
				array[7].page_h->free = 0; 
				array[7].page_h->next->next = NULL; 
				array[7].page_h->size = size + HEAD_SIZE; 
				return array[7].page_h->next->limit; 
		}
		else
		{
				pt_page_h tmp_s = array[7].page_h; 
				while(tmp_s->next_h)
						tmp_s = tmp_s->next_h; 
			if ((	tmp_s->next_h =  mmap(NULL, size + HEAD_SIZE, PROT_WRITE 
								| PROT_READ, MAP_ANON | MAP_PRIVATE, 0, 0)) == (void*) -1)
             return NULL; 
				tmp_s->free = 0; 
				tmp_s = tmp_s->next_h; 
				tmp_s->size = size + HEAD_SIZE; 
				tmp_s->next_h = NULL; 
				tmp_s->next = (pt_block)(tmp_s->limit); 
    tmp_s->next->head = tmp_s; 
				tmp_s->next->free = 0; 
				return tmp_s->next->limit; 
		}
}

          // this function search a free page else create a new free page 
void *find_page(size_t size)
{
		pt_page_h head = array[7].page_h; 
		while(head && (head->size <= size) && !head->free)
				    head = head->next_h; 
  if (!head)
         return allocate_page(size);    
		return head->next->limit; 
}

          //if there no more space this function create a new 
          //page use with the best fit algorithm
void *new_page(pt_page_h head, int page_number)
{
		pt_page_h tmp_s = head; 
		while (tmp_s->next_h)
				tmp_s = tmp_s->next_h; 
		if ((tmp_s->next_h = mmap(NULL, PAGE_SIZE, PROT_WRITE 
						| PROT_READ, MAP_PRIVATE | MAP_ANON, 0, 0)) == (void*)-1)
            return NULL; 
		tmp_s = tmp_s->next_h;
		tmp_s->full = 0; 
		tmp_s->head_page = (pt_block)tmp_s; 
		tmp_s->next = (pt_block)(tmp_s->limit + size_page_block(page_number));
  tmp_s->next->head = tmp_s; 
		tmp_s->next_h = NULL; 
		//     tmp_s->free = 1; 
		segment_page(size_page_block(page_number),tmp_s); 
		tmp_s->next->free = 0; 
		return tmp_s->next->limit; 
}


void create_pages(const int page_number)
{
          array[page_number].page_h = mmap(NULL, PAGE_SIZE, PROT_WRITE 
								| PROT_READ, MAP_ANON | MAP_PRIVATE, 0, 0);
          array[page_number].size = size_page_block(page_number); 
          array[page_number].page_h->head_page = (pt_block)array[page_number].page_h; 
          array[page_number].page_h->full = 0; 
          array[page_number].page_h->next = (pt_block)(array[page_number].page_h->limit + array[page_number].size);
          array[page_number].page_h->next->head = array[page_number].page_h; 
          array[page_number].page_h->next_h = NULL;
          segment_page(array[page_number].size, array[page_number].page_h);
}

void *find_block(size_t size)
{
          int page = 0; 
          PAGE_NUMBER(size, page) 
          pt_page_h page_head = array[page].page_h; 
          if (!array[page].page_h)
                {
                    first[page] = 1; 
                    create_pages(page); 
                    return new_page(array[page].page_h, page);
                }
          while (page_head->full && page_head->next_h)
                    page_head = page_head->next_h; 
          pt_block block =  page_head->next; 
          while (block->next && !block->free)
                    block = block->next;
          if (!(block->next && block->free))
                {
                    page_head->full = 1; 
                    return new_page(array[page].page_h, page);
                }
          block->free = 0;
          return block->limit; 
}

void segment_page(size_t size, pt_page_h head)
{ 
		size_t  counter = 1;  
		pt_block block = head->next; 
		while (counter < NBR_BLOCK(size))
		{
				block->free = 1; 
    block->head = head; 
				block->next = (pt_block)(block->limit + size); 
    block = block->next; 
				counter++; 
		}
		block->next = NULL; 
}

void *handle_page(pt_page_h head, void *ptr, size_t size)
{
		long diff  =  ((((head->size) / PAGE_SIZE) + 1) * PAGE_SIZE) -  size;
		if (diff >= 0)
		{
				head->size = size + HEAD_SIZE; 
				return head->next->limit; 
		}
		else
		{
				void *new_ptr = find_page(size); 
				memmove(new_ptr, ptr, size); 
				free(ptr); 
				return new_ptr; 
		}
}

void *realloc_block(size_t size, void *ptr)
{
		size = ALIGN_BSIZE(size);
		void* tmp = ((char *)ptr - 24); 
   pt_block block = (pt_block) (tmp); 
		if (block->head->size >= 512)
				return handle_page(block->head, ptr, size);
		else
		{
				if (block->head->size >= size)
						return ptr;
				void *new_ptr = NULL;
				if (size > 512)
					  	new_ptr = find_page(size); 
				else
              new_ptr = find_block(size); 
              memmove(new_ptr, ptr, size);
              free(ptr); 
     return new_ptr; 
  }
}
