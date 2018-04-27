/*
 * linker.c
 *
 *  Created on: Apr 20, 2018
 *      Author: connor
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../linker.hpp"
#ifndef __ELF__
#include <windows.h>
#endif

#ifndef __ELF__
static HINSTANCE *handles;
static int nhandles;
static char dll_list[] = DLLS;
#endif

static int dll_listify(char *str, char ***out) {
	*out = (char **) calloc(0, sizeof(char *));
	int prev = 0;
	int i = 0;
	int count = 0;
	int squoted = 0, dquoted = 0, escaped = 0, trim = 1;
	while(str[i] != 0) {
		switch(str[i]) {
		case '\\' :
			escaped = !escaped;
			trim = 0;
			break;
		case '"':
			if(!squoted && !escaped) {
				dquoted = !dquoted;
			}
			trim = 0;
			break;
		case '\'' :
			if(!dquoted && !escaped) {
				squoted = !squoted;
			}
			trim = 0;
			break;
		case ' ':
		case '\t':
		case '\n':
			if(escaped || squoted || dquoted) {
				trim = 0;
				break;
			}
			if(trim) {
				prev++;
				break;
			}
			trim = 1;
			str[i] = 0;

			*out = (char **) realloc(*out, (count + 1) * sizeof(char *));
			(*out)[count] = &(str[prev]);
			prev = i + 1;
			count++;
			break;
		default:
			trim = 0;
			break;
		}
		i++;
	}
	*out = (char **) realloc(*out, (count + 1) * sizeof(char *));
	(*out)[count] = &(str[prev]);
	count++;
	return (count);
}

void linkDLLs(void) {
#ifndef __ELF__
	int argc;
	char **argv;
	argc = dll_listify(dll_list, &argv);
	handles = (HINSTANCE *) calloc(argc, sizeof(HINSTANCE));
	nhandles = argc;
	for(int i = 0; i < argc; i++) {
		handles[i] = LoadLibrary(argv[i]);
	}
	free(argv);
#else
	return;
#endif
}

void unlinkDLLs(void) {
#ifndef __ELF__
	for(int i = 0; i < nhandles; i++) {
		FreeLibrary(handles[i]);
	}
	free(handles);
#else
		return;
#endif
}
