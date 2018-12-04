/*
 * dimension.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: connor
 */

#include "arrays.hpp"

constexpr compchem::Dimension::Dimension(int dim0, int dim1 = 0, int dim2 = 0, int dim3 = 0,
		int dim4 = 0, int dim5 = 0, int dim6 = 0) {
	shape = {dim0, dim1, dim2, dim3, dim4, dim5, dim6};
}

int compchem::Dimension::operator[](int ind) const {
	return (shape[ind]);
}

bool operator==(compchem::Dimension dim1, compchem::Dimension dim2) const {
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

