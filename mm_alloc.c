/* Assignment 5
 * Operating Systems
 * Kinneret College on the Sea of Galilee
 * Semester 1 5781
 * 
 * Submitted by: Alexandr Chernin & Shimon Yaish
 *
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include <unistd.h>
#include <string.h>
#include "mm_alloc.h"
#include <stdlib.h>

#define ALLOC_MIN		3 * sysconf(_SC_PAGESIZE)
#define DEALLOC_MIN		1 * sysconf(_SC_PAGESIZE)
#define BLOCK_PAYLOAD(p)	((void*)((unsigned long)p + sizeof(mem_block)))
#define BLOCK_METADATA(p)	((void*)((unsigned long)p - sizeof(mem_block)))

/*the two functions above transofms the pointer to the memory block, either to the allocated space or its metadata*/


typedef struct mem_block {
	size_t			size;
	struct mem_block*	next;
	struct mem_block*	prev;
} mem_block;

/* linked list for the metadata */

mem_block* head = NULL;

/*this function deletes memory block from the list of free blocks*/
void _set_allocated(mem_block* block){
	if (!block->prev){
		if (block->next)	head = block->next;
		else			head = NULL;
	}else
		block->prev->next = block->next;
	if (block->next)
		block->next->prev = block->prev;
}

/*this functions adds block to list of avaliables, meanwhile sorting the list*/
void _set_free(mem_block* block){
	block->prev = NULL;
	block->next = NULL;
	if (!head || (unsigned long)head > (unsigned long)block){
		if (head) head->prev = block;
		block->next = head;
		head = block;
	}else{
		mem_block* current = head;
		while(current->next && (unsigned long)current->next < (unsigned long)block)
			current = current->next;
		block->next = current->next;
		current->next = block;
	}

}

/*sorting the list, combining avaliable blocks and returning free blocks at the end of the list back to system*/
void _rearm(){
	mem_block*	current = head;
	unsigned long	current_header, next_header;
	unsigned long	programm_break = (unsigned long)sbrk(0);
	while (current->next){
		current_header	= (unsigned long)current;
		next_header	= (unsigned long)current->next;
		if (current_header + current->size + sizeof(mem_block) == next_header){
			current->size += current->next->size + sizeof(mem_block);
			current->next = current->next->next;
			if (current->next) {
				current->next->prev = current;
			} else	break;
		}
		current = current->next;
	}
	current_header = (unsigned long)current;
	if (current_header + current->size + sizeof(mem_block) == programm_break && current->size >= DEALLOC_MIN)
		_set_allocated(current);
}

/*spliting big block into block of the right size(returning it) and remaining size (goes to list)*/
mem_block* _split_block(mem_block* block, size_t size){
	void*		mm_block	= BLOCK_PAYLOAD(block);
	mem_block*	newptr		= (mem_block*) ((unsigned long)mm_block + size);
	newptr->size = block->size - (size + sizeof(mem_block));
	block->size = size;
	return newptr;
}


/*
1. searching block of the right size or bigger size
2. if we found block of the right size - filling it with zeros and returning pointer to it
3. if we found block of bigger size than needed, we split it, filling it with zeros and returning pointer to it
4. if we didnt found block - we ask the system to add more space
5. we split the space from system, filling it with zeros and returning pointer to it
*/
void *mm_malloc(size_t size) {
	if (size == 0) return NULL;
	void*		block;
	mem_block	*ptr, *newptr;
	size_t		alloc_size = size >= ALLOC_MIN ? size + sizeof(mem_block) : ALLOC_MIN;
	ptr = head;
	while (ptr){
		if (ptr->size >= size + sizeof(mem_block)){
			block = BLOCK_PAYLOAD(ptr);
			_set_allocated(ptr);
			if (ptr->size == size)
				return memset(block, 0, size);
			newptr = _split_block(ptr, size);
			_set_free(newptr);
			return memset(block, 0, size);
		}else	ptr = ptr->next;
	}
	ptr = sbrk(alloc_size);
	if (!ptr) return NULL;
	ptr->next = NULL;
	ptr->prev = NULL;
	ptr->size = alloc_size - sizeof(mem_block);
	if (alloc_size > size + sizeof(mem_block)){
		newptr = _split_block(ptr, size);
		_set_free(newptr);
	}
	return memset(BLOCK_PAYLOAD(ptr), 0, size);
}


/* 
 
 1. creating new block of the right size
 2. moving data to the new block
 3. freeing the old block
 */
void *mm_realloc(void *ptr, size_t size) {
	if (ptr == NULL && size == 0)	
		return NULL;
	if (ptr == NULL)		
		return mm_malloc(size);
	if (size == 0)			{
		mm_free(ptr); return NULL;}
	void* block = mm_malloc(size);
	if (block == NULL) return NULL;
	memcpy(block, ptr, size);
	mm_free(ptr);
	return block;
}

/* 
 * 1. doing nothing if NULL
 * 2. adding block to the list of free blocks 
 * 3. returning memory to the system
 */

void mm_free(void *ptr) {
	if (ptr == NULL) return;
	_set_free(BLOCK_METADATA(ptr));
	_rearm();
}
