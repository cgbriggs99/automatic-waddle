/*
 * free.c
 *
 *  Created on: Nov 8, 2017
 *      Author: cgbri
 */

#include <stdlib.h>
#include <stdarg.h>
#include "./mem_custom.h"

void free_mult(int args, ...) {
	va_list lst;

	va_start(lst, args);

	for(int i = 0; i < args; i++) {
		void *ptr = va_arg(lst, void *);
		free(ptr);
	}
	va_end(lst);
}
