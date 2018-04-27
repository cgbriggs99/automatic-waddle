/*
 * input.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <stdio.h>
#include "molecule.hpp"

namespace compchem {
/*
 * Get input from a file and place it in out.
 */
extern void input(Molecule **out, FILE *fp);

/*
 * Read in Hessian data.
 */
extern void inputHessian(Molecule **out, FILE *fp);

/*
 * Read data from various files for the SCF procedure.
 */
extern void inputSCF(Molecule **out, FILE *mol, FILE *enuc, FILE *s, FILE *t,
		FILE *v, FILE *eri, FILE *mux, FILE *muy, FILE *muz);
}


#endif /* INPUT_HPP_ */
