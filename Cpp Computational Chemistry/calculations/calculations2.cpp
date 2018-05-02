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

using namespace std;
using namespace compchem;
using namespace array;

static int compare(const void *a, const void *b) {
	if(*((double *) a) - *((double *) b) > 0) {
		return (1);
	} else if(*((double *) a) - *((double *) b) < 0) {
		return (-1);
	}
	return (0);
}

void compchem::Molecule::harmonics() {
	for (int i = 0; i < this->hessian_size; i++) {
		for (int j = 0; j < this->hessian_size; j++) {
			(*(this->hessian))(i, j) /= sqrt(
					this->atoms[i / 3].mass * this->atoms[j / 3].mass);
		}
	}
	double *im = (double *) malloc(this->hessian_size * sizeof(double));
	LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'N', this->hessian_size,
			this->hessian->getArray(), this->hessian_size,
			this->hessian_eigs->getArray(), im, NULL, this->hessian_size, NULL, this->hessian_size);

	qsort(this->hessian_eigs->getArray(), this->hessian_size, sizeof(double), compare);

	for(int i = 0; i < this->hessian_size; i++) {
		(*(this->vibrations))(i) = sqrt((*(this->hessian_eigs))(i)) * 5140.697669352;
	}
}
#endif
