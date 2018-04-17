/*
 * mem_custom.h
 *
 *  Created on: Nov 8, 2017
 *      Author: cgbri
 */

#ifndef MEM_CUSTOM_H_
#define MEM_CUSTOM_H_

#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

/*
 * Allocate a block, with error checking.
 */
extern void *malloc_check(size_t size);
extern void *calloc_check(size_t dim, size_t elem);
extern void *realloc_check(void *ptr, size_t newsize);

/*
 * Allocate a contiguous block of memory.
 */
extern void *malloc_contiguous(int dim, size_t elem, ...);
extern void *calloc_contiguous(int dim, size_t elem, ...);

/*
 * Free blocks.
 */
extern void free_mult(int args, ...);
extern void free_contig(void *cont);
extern void free_mult_contig(int args, ...);

#endif /* MEM_CUSTOM_H_ */
