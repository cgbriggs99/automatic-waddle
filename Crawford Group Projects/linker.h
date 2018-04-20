/*
 * linker.h
 *
 *  Created on: Apr 20, 2018
 *      Author: connor
 */

#ifndef LINKER_H_
#define LINKER_H_

const char dll_str[] = DLLS;

extern void linkDLLs(int argc, char **argv);

extern void unlinkDLLs(void);

#endif /* LINKER_H_ */
