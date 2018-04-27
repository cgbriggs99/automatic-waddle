/*
 * calculations2.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: Connor
 */

#include "../output.hpp"
#include "../arrays.hpp"
#include "../input.hpp"
#include "../molecule.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace compchem;
using namespace array;

void Molecule::harmonics() {
	for (int i = 0; i < this->hessian_size; i++) {
		for (int j = 0; j < this->hessian_size; j++) {
			(*(this->hessian))(i, j) /= sqrt(
					this->atoms[i / 3].mass * this->atoms[j / 3].mass);
		}
	}
	double *im = (double *) malloc(this->hessian_size * sizeof(double));
	LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'N', this->hessian_size,
			this->hessian->getArray(), this->hessian_size,
			this->hessian_eigs->getArray(), NULL, 1, NULL, 1);

	for(int i = 0; i < this->hessian_size; i++) {
		(*(this->vibrations))(i) = sqrt((*(this->hessian_eigs))(i)) * 5140.697669352;
	}
}

