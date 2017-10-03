/*
 * mmem.h
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */

#ifndef MAC_DIRVER_MMEM_H_
#define MAC_DIRVER_MMEM_H_

#include "mac.h"
#define MMEM_PTR(m) (struct mmem *)(m)->ptr




int  mmem_alloc(struct mmem *m, unsigned int size);
void mmem_free(struct mmem *);
void mmem_init(void);



#endif /* MAC_DIRVER_MMEM_H_ */
