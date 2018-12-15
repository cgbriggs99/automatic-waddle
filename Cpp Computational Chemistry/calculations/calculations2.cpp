/*
 * calculations2.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: Connor
 */
#ifndef CALCULATIONS2_CPP
#define CALCULATIONS2_CPP
#include "../compchem.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lapacke.h>

using namespace std;
using namespace compchem;

static int compare(const void *a, const void *b) {
	if(*((double *) a) - *((double *) b) > 0) {
		return (1);
	} else if(*((double *) a) - *((double *) b) < 0) {
		return (-1);
	}
	return (0);
}

void compchem::Molecule::harmonics() {
	for(int i = 0; i < this->hessian->getShape()[0]; i++) {
		for(int j = 0; j < this->hessian->getShape()[0]; j++) {
			this->hessian->subscript(i, j) /= sqrt(
			    this->atoms[i / 3].getMass() * this->atoms[j / 3].getMass());
		}
	}
	compchem::FortranArray<double> im(this->hessian_eigs->getShape());
	LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'N', this->hessian->getShape()[0],
	    this->hessian->getData(), this->hessian->getShape()[0],
	    this->hessian_eigs->getData(), im.getData(), NULL,
	    this->hessian->getShape()[0],
	    NULL, this->hessian->getShape()[0]);

	qsort(this->hessian_eigs->getData(), this->hessian->getShape()[0],
	    sizeof(double), compare);

	for(int i = 0; i < this->hessian->getShape()[0]; i++) {
		this->vibrations->subscript(i) = sqrt(this->hessian_eigs->subscript(i))
		    * 5140.697669352;
	}
}
#endif
