/*
 * linker.c
 *
 *  Created on: Apr 20, 2018
 *      Author: connor
 */

#include <stdio.h>
#ifndef __ELF__
#include <windows.h>
#endif

#ifndef __ELF__
static HINSTANCE *handles;
static int nhandles;
#endif

void linkDLLs(int argc, char **argv) {
#ifndef __ELF__
	handles = calloc(argc, sizeof(HINSTANCE *));
	nhandles = argc;
	for(int i = 0; i < argc; i++) {
		handles[i] = LoadLibrary(argv[i]);
	}
#endif
}

void unlinkDLLs(void) {
#ifndef __ELF__
	for(int i = 0; i < nhandles; i++) {
		FreeLibrary(handles[i]);
	}
	free(handles);
#endif
}
