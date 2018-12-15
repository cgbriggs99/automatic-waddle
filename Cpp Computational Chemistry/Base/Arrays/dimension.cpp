/*
 * dimension.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: connor
 */



#ifndef __DIMENSION_CPP__
#define __DIMENSION_CPP__

#include "arrays.hpp"

bool compchem::operator==(compchem::Dimension dim1, compchem::Dimension dim2) {
	for(int i = 0; i < 7; i++) {
		if(dim1[i] != dim2[i]) {
			return (false);
		}
	}
	return (true);
}

int compchem::Dimension::getDimensionality() {
	for(int i = 0; i < 7; i++) {
		if(shape[i] == 0) {
			return (i);
		}
	}
	return (7);
}

bool compchem::operator!=(compchem::Dimension dim1, compchem::Dimension dim2) {
	for(int i = 0; i < 7; i++) {
		if(dim1[i] != dim2[i]) {
			return (true);
		}
	}
	return (false);
}

compchem::Dimension &compchem::Dimension::operator=(compchem::Dimension const &dim) {
	for(int i = 0; i < 7; i++) {
		this->shape[i] = dim.shape[i];
	}
	return (*this);
}

#endif
