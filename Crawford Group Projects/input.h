/*
 * input.h
 *
 *  Created on: Sep 5, 2017
 *      Author: connor
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>
#include "./molecule.h"

#ifdef __cplusplus
extern "C" {
struct molecule_t;
#endif

/*
 * Get input from a file and place it in out.
 */
extern void input(molecule_t **out, FILE *fp);

/*
 * Read in Hessian data.
 */
extern void inputHessian(molecule_t **out, FILE *fp);

/*
 * Read data from various files for the SCF procedure.
 */
extern void inputSCF(molecule_t **out, FILE *mol, FILE *enuc, FILE *s, FILE *t,
		FILE *v, FILE *eri, FILE *mux, FILE *muy, FILE *muz);

#ifdef __cplusplus
}
#endif
#endif /* INPUT_H_ */
