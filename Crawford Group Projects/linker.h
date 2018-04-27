/*
 * linker.h
 *
 *  Created on: Apr 20, 2018
 *      Author: connor
 */

#ifndef LINKER_H_
#define LINKER_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void linkDLLs(void);

extern void unlinkDLLs(void);

#ifdef __cplusplus
}
#endif

#endif /* LINKER_H_ */
