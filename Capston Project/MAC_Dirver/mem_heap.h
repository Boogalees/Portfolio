/*
 * mem_heap.h
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */

#ifndef MAC_DIRVER_MEM_HEAP_H_
#define MAC_DIRVER_MEM_HEAP_H_

#include "mmem.h"
#include "mac.h"
#include "constants.h"


#define MAX_MEM_PTR_POOL 30             ///< Define the number of mem pointers here



void mem_heap_init();
mem_ptr_t *mem_heap_alloc(uint8 size);
void mem_heap_free(mem_ptr_t *mem_ptr);



#endif /* MAC_DIRVER_MEM_HEAP_H_ */
