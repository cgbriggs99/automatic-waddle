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

void compchem::Molecule::harmonics() {
	for(int i = 0; i < this->hessian->getShape()[0]; i++) {
		for(int j = 0; j < this->hessian->getShape()[0]; j++) {
			this->hessian->subscript(i, j) /= sqrt(
			    this->atoms[i / 3].getMass() * this->atoms[j / 3].getMass());
		}
	}

	compchem::eigenval_compute(false, false, *(this->hessian), this->hessian_eigs, nullptr, nullptr, nullptr);

	this->hessian_eigs->sort();

	for(int i = 0; i < this->hessian->getShape()[0]; i++) {
		this->vibrations->subscript(i) = sqrt(this->hessian_eigs->subscript(i))
		    * 5140.697669352;
	}
}
#endif
