/*
 * alloc.c
 *
 *  Created on: Nov 8, 2017
 *      Author: cgbri
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "./mem_custom.h"

void *malloc_check(size_t size) {
	errno = 0;
	void *out;
	out = malloc(size);
	if(out == NULL && errno == ENOMEM) {
		perror("Out of memory!\n");
		exit(-1);
	}
	return (out);
}

void *calloc_check(size_t dim, size_t elem) {
	errno = 0;
	void *out;
	out = calloc(dim, elem);
	if(out == NULL && errno == ENOMEM) {
		perror("Out of memory!\n");
		exit(-1);
	}
	return (out);
}

void *realloc_check(void *ptr, size_t newsize) {
	errno = 0;
	void *out;
	out = realloc(ptr, newsize);
	if(out == NULL && errno == ENOMEM) {
		perror("Out of memory! Using input as output\n");
		return (ptr);
	}
	return (out);
}
