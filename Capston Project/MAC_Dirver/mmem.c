/*
 * mmem.c
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */
#include "mmem.h"
#include "constants.h"
#include "string.h"
#include "list.h"


#define MMEM_SIZE 2048   // should change to CC2538 datasheet

LIST(mmemlist);

unsigned int avail_memory;
static char memory[MMEM_SIZE];


/*---------------------------------------------------------------------------*/
/**
 * \brief      Allocate a managed memory block
 * \param m    A pointer to a struct mmem.
 * \param size The size of the requested memory block
 * \return     Non-zero if the memory could be allocated, zero if memory
 *             was not available.
 * \author     Adam Dunkels
 *
 *             This function allocates a chunk of managed memory. The
 *             memory allocated with this function must be deallocated
 *             using the mmem_free() function.
 *
 *             \note This function does NOT return a pointer to the
 *             allocated memory, but a pointer to a structure that
 *             contains information about the managed memory. The
 *             macro MMEM_PTR() is used to get a pointer to the
 *             allocated memory.
 *
 */
int
mmem_alloc(struct mmem *m, unsigned int size)
{
  /* Check if we have enough memory left for this allocation. */
  if(100 < size) {
    return 0;
  }

  /* We had enough memory so we add this memory block to the end of
     the list of allocated memory blocks. */
  list_add(mmemlist, m);

  /* Set up the pointer so that it points to the first available byte
     in the memory block. */
  m->ptr = &memory[MMEM_SIZE - avail_memory];

  /* Remember the size of this memory block. */
  m->size = size;

  /* Decrease the amount of available memory. */
  avail_memory -= size;

  /* Return non-zero to indicate that we were able to allocate
     memory. */
  return 1;
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      Deallocate a managed memory block
 * \param m    A pointer to the managed memory block
 * \author     Adam Dunkels
 *
 *             This function deallocates a managed memory block that
 *             previously has been allocated with mmem_alloc().
 *
 */
void
mmem_free(struct mmem *m)
{
  struct mmem *n;

  if(m->next != NULL) {
    /* Compact the memory after the allocation that is to be removed
       by moving it downwards. */
    memmove(m->ptr, m->next->ptr,
	    &memory[MMEM_SIZE - avail_memory] - (char *)m->next->ptr);

    /* Update all the memory pointers that points to memory that is
       after the allocation that is to be removed. */
    for(n = m->next; n != NULL; n = n->next) {
      n->ptr = (void *)((char *)n->ptr - m->size);
    }
  }

  avail_memory += m->size;

  /* Remove the memory block from the list. */
  list_remove(mmemlist, m);
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      Initialize the managed memory module
 * \author     Adam Dunkels
 *
 *             This function initializes the managed memory module and
 *             should be called before any other function from the
 *             module.
 *
 */
void
mmem_init(void)
{
  list_init(mmemlist);
  avail_memory = MMEM_SIZE;
}
