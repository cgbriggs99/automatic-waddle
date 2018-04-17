/*
 * contig.c
 *
 *  Created on: Nov 8, 2017
 *      Author: cgbri
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include "./mem_custom.h"
#include <string.h>

/*
 * | malloc stuff | block data | block |
 */
typedef struct __contig_header_t__ {
	size_t dim, size;	//Size and depth of the block
	int pid, zero;//Identification marks. These say that this is a special block.
} contig_header_t;

void *malloc_contiguous(int dim, size_t elem, ...) {
	va_list lst1;
	void *out, *curr;
	contig_header_t *header;
	size_t sum = 0, prod = 1;
	size_t *dims;

	if(dim <= 0) {
		return (NULL);
	}

	va_start(lst1, elem);

	dims = calloc(dim, sizeof(size_t));

	if(dim == 1) {
		size_t s = va_arg(lst1, size_t);
		va_end(lst1);
		return (malloc_check(s * elem));
	}

	for(int i = 0; i < dim; i++) {
		size_t s = va_arg(lst1, size_t);
		dims[i] = s;
		if(i < dim - 1) {
			sum += prod * s * sizeof(void *);
		} else {
			sum += prod * s * elem;
		}
		prod *= s;
	}
	va_end(lst1);

	header = malloc_check(sizeof(contig_header_t) + sum);

	header->dim = dim;
	header->pid = getpid();
	header->zero = 0;
	header->size = sum;

	curr = out = (header + 1);
	prod = 1;

	for(int i = 0; i < dim - 1; i++) {
		prod *= dims[i];
		*((void **) curr) = curr + dims[i] * sizeof(void *);

		for(int j = 1; j < dims[i]; j++) {
			if(i != dim - 2) {
				*((void **) curr + j) = *((void **) curr + j - 1)
				    + dims[i + 1] * sizeof(void *);
			} else {
				*((void **) curr + j) = *((void **) curr + j - 1)
				    + dims[i + 1] * elem;
			}
		}
		curr = *((void **) curr);
	}
	va_end(lst1);
	free(dims);
	return (out);
}

void *calloc_contiguous(int dim, size_t elem, ...) {
	va_list lst1;
	void *out, *curr;
	contig_header_t *header;
	size_t sum = 0, prod = 1;
	size_t *dims;

	if(dim <= 0) {
		return (NULL);
	}

	va_start(lst1, elem);

	dims = calloc(dim, sizeof(size_t));

	if(dim == 1) {
		size_t s = va_arg(lst1, size_t);
		va_end(lst1);
		return (malloc_check(s * elem));
	}

	for(int i = 0; i < dim; i++) {
		size_t s = va_arg(lst1, size_t);
		dims[i] = s;
		if(i < dim - 1) {
			sum += prod * s * sizeof(void *);
		} else {
			sum += prod * s * elem;
		}
		prod *= s;
	}
	va_end(lst1);

	out = calloc_check(sum, 1);
//
//	header->dim = dim;
//	header->pid = getpid();
//	header->zero = 0;
//	header->size = sum;

	curr = out;// = (header + 1);
	prod = 1;

	for(int i = 0; i < dim - 1; i++) {
		prod *= dims[i];
		*((void **) curr) = curr + dims[i] * sizeof(void *);

		for(int j = 1; j < dims[i]; j++) {
			if(i != dim - 2) {
				*((void **) curr + j) = *((void **) curr + j - 1)
				    + dims[i + 1] * sizeof(void *);
			} else {
				*((void **) curr + j) = *((void **) curr + j - 1)
				    + dims[i + 1] * elem;
			}
		}
		curr = *((void **) curr);
	}
	va_end(lst1);
	free(dims);
	return (out);
}

void free_contig(void *cont) {
//	contig_header_t *header;
//	header = cont - sizeof(contig_header_t);
//
//	if(header->zero == 0
//	    && (header->pid == getpid())) {
//		header->pid = 0x00bada55;	//Clear the sequences to avoid confusion.
//		header->zero = 0xdeadbeef;
//		memset(cont, 0, header->size);	//Clear the data so that the
//		                                //keywords don't appear.
//		free(header);
//	} else {
//		free(cont);
//	}
	free(cont);
}

void free_mult_contig(int args, ...) {
	va_list lst;
	va_start(lst, args);

	for(int i = 0; i < args; i++) {
		free_contig(va_arg(lst, void *));
	}
	va_end(lst);
}
