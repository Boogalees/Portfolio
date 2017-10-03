/*
 * mem_heap.c
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */


#include "mem_heap.h"
#include "stdio.h"
#include "string.h"
/**************************************************************************/
/*!
        The mem pointer pool contains an array of all the memory pointers that
        are available to the stack. Each memory pointer can contain one block
        of memory allocated from the heap. Needless to say, if we run out of
        memory pointers, then that is just as bad as running out of memory.
*/
/**************************************************************************/
static mem_ptr_t mem_ptr_pool[MAX_MEM_PTR_POOL];
/**************************************************************************/
/*!
        Initialize the total memory heap.
*/
/**************************************************************************/
void mem_heap_init()
{
    memset(mem_ptr_pool, 0, MAX_MEM_PTR_POOL * sizeof(mem_ptr_t));
}

/**************************************************************************/
/*!
    Find a free memory pointer. If one is located, then allocate the managed
    memory from the memory heap. If no memory is available, then return NULL.
*/
/**************************************************************************/
mem_ptr_t *mem_heap_alloc(uint8 size)
{
    uint8 i;

    for (i=0; i<MAX_MEM_PTR_POOL; i++)
    {
        if (!mem_ptr_pool[i].alloc)
        {
            // found a free mem ptr. only mark it used if we can alloc memory to it.
            if (mmem_alloc(&mem_ptr_pool[i].mmem_ptr, size))
            {
                // memory successfully alloc'd. clear the block, mark this sucker used, and return it.
                memset(mem_ptr_pool[i].mmem_ptr.ptr, 0, sizeof(mem_ptr_t));
                mem_ptr_pool[i].alloc = 1;
                return &mem_ptr_pool[i];
            }
            else
            {
                // no more memory. don't touch the ptr and return NULL.
                return NULL;
            }
        }
    }
    // couldn't find any free mem pointers. return NULL.
    return NULL;
}

/**************************************************************************/
/*!
    Free the managed memory and then de-allocated the memory pointer.
*/
/**************************************************************************/
void mem_heap_free(mem_ptr_t *mem_ptr)
{
    if (mem_ptr)
    {
        mmem_free(&mem_ptr->mmem_ptr);
        mem_ptr->alloc = 0;
    }
}
