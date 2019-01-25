/*
 * arrays_compiled.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: connor
 */

#include "arrays.hpp"
#include <lapacke.h>

template<>
compchem::FortranArray<double>::FortranArray(compchem::Dimension dim) {
	this->dim = dim;
	this->data = new double[this->getSize()];
}

template<>
compchem::FortranArray<double>::FortranArray(compchem::Dimension dim,
		std::initializer_list<double> list) {
	this->dim = dim;
	this->data = new double[this->getSize()];
	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] = list.begin()[i];
	}
}

template<>
compchem::FortranArray<double>::FortranArray(std::initializer_list<double> list) {
	this->dim = list.size();
	this->data = new double[this->getSize()];
	for (int i = 0; i < list.size(); i++) {
		this->data[i] = list.begin()[i];
	}
}

template<>
compchem::FortranArray<double>::FortranArray(compchem::Dimension dim, double *values) {
	this->dim = dim;
	this->data = new double[this->getSize()];
	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] = values[i];
	}
}

template<>
compchem::FortranArray<double>::FortranArray(const FortranArray<double> &arr) {
	this->dim = arr.dim;
	this->data = new double[this->getSize()];
	for (int i = 0; i < this->getSize(); i++) {
		//Copy.
//		new(&(this->data[i])) double(&(arr.data[i]));
		this->data[i] = arr.data[i];
	}
}

template<>
compchem::FortranArray<double>::~FortranArray() {
 	delete[] (this->data);
}

template<>
double &compchem::FortranArray<double>::operator()(int ind0, int ind1, int ind2,
		int ind3, int ind4, int ind5, int ind6) {
	int ind = 0;
	int size = 1;
	std::array<int, 7> inds = { ind0, ind1, ind2, ind3, ind4, ind5, ind6 };

	for(int i = 0; i < this->getShape().getDimensionality(); i++) {
		if(inds[i] >= dim[i] || inds[i] < 0) {
			throw(new ArrayIndexOutOfBoundsException);
		}
		ind += inds[i] * size;
		size *= dim[i];
	}
	return (data[ind]);
}

template<>
const double &compchem::FortranArray<double>::operator()(int ind0, int ind1, int ind2,
		int ind3, int ind4, int ind5, int ind6) const {
	int ind = 0;
	int size = 1;
	std::array<int, 7> inds = { ind0, ind1, ind2, ind3, ind4, ind5, ind6 };

	for(int i = 0; i < this->getShape().getDimensionality(); i++) {
		if(inds[i] >= dim[i] || inds[i] < 0) {
			throw(new ArrayIndexOutOfBoundsException);
		}
		ind += inds[i] * size;
		size *= dim[i];
	}
	return (data[ind]);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator=(
		const FortranArray<double> &b) {
	delete[] this->data;

	this->dim = b.dim;
	this->data = new double[b.getSize()];

	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] = b.data[i];
	}

	return (*this);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator+=(
		const FortranArray<double> &b) {
	if (this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] += b.data[i];
	}

	return (*this);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator-=(
		const FortranArray<double> &b) {
	if (this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] -= b.data[i];
	}

	return (*this);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator*=(
		const FortranArray<double> &b) {
	if (this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] *= b.data[i];
	}

	return (*this);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator*=(
		const double &b) {
	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] *= b;
	}

	return (*this);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator/=(
		const double &b) {
	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] /= b;
	}

	return (*this);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::operator/=(
		const FortranArray<double> &b) {
	if (this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for (int i = 0; i < this->getSize(); i++) {
		this->data[i] /= b.data[i];
	}
	return (*this);
}

template<>
compchem::FortranArray<double> compchem::operator+(compchem::FortranArray<double> a,
		const compchem::FortranArray<double> &b) {
	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.dim);

	if (a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for (int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] + b.data[i];
	}

	return (*out);
}

template<>
compchem::FortranArray<double> compchem::operator-(compchem::FortranArray<double> a,
		const compchem::FortranArray<double> &b) {

	if (a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}
	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.getShape());

	for (int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] - b.data[i];
	}

	return (*out);
}

template<>
compchem::FortranArray<double> compchem::operator*(compchem::FortranArray<double> a,
		const double &k) {
	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.dim);

	for (int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] * k;
	}

	return (*out);
}

template<>
compchem::FortranArray<double> compchem::operator*(double k, const compchem::FortranArray<double> &a) {
	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.dim);

	for(int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] * k;
	}

	return (*out);
}

//Standard Fortran * operator. Not matrix multiplication.
template<>
compchem::FortranArray<double> compchem::operator*(compchem::FortranArray<double> a,
		const compchem::FortranArray<double> &b) {
	if (a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.dim);

	for (int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] * b.data[i];
	}

	return (*out);
}

template<>
compchem::FortranArray<double> compchem::operator/(compchem::FortranArray<double> a,
		const compchem::FortranArray<double> &b) {
	if (a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.dim);

	for (int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] / b.data[i];
	}

	return (*out);
}

template<>
compchem::FortranArray<double> compchem::operator/(compchem::FortranArray<double> a,
		double k) {
	compchem::FortranArray<double> *out = new compchem::FortranArray<double>(a.dim);

	for (int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] / k;
	}

	return (*out);
}

template<>
double compchem::FortranArray<double>::dotprod(FortranArray<double> &b) {
	if (this->getShape(0) != b.getShape(0)) {
		throw(new compchem::SizeMismatchException);
	}
	double accum = (*this)(0) * b(0);
	for (int i = 1; i < this->getShape(0); i++) {
		accum += (*this)(i) * b(i);
	}
	return (accum);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::crossprod(
		compchem::FortranArray<double> &b) {
	if (this->getShape(0) != 3 || b.getShape(0) != 3) {
		throw(new compchem::CrossProductException);
	}
	compchem::FortranArray<double> *out = new FortranArray<double>(Dimension(3));
	(*out)(0) = (*this)(1) * b(2) - (*this)(2) * b(1);
	(*out)(1) = (*this)(0) * b(2) - (*this)(2) * b(0);
	(*out)(2) = (*this)(0) * b(1) - (*this)(1) * b(0);

	return (*out);
}

static int partition_work(double *array, int low, int high) {
	double &pivot = array[high];
	int i = low;
	for(int j = low; j < high - 1; j++) {
		if(array[i] < pivot) {
			if(i != j) {
				std::swap(array[i], array[j]);
			}
			i++;
		}
	}
	std::swap(array[i], array[high]);
	return (i);
}


static void sort_work(double *array, int low, int high) {
	if(low < high) {
		int p = partition_work(array, low, high);
		sort_work(array, low, p - 1);
		sort_work(array, p + 1, high);
	}
}

static int compare(const void *a, const void *b) {
	if(*(double *) a > *(double *) b) {
		return (1);
	} else if(*(double *) b > *(double *) a) {
		return (-1);
	} else {
		return (0);
	}
}

template<>
void compchem::FortranArray<double>::sort() {
	if(this->getShape().getDimensionality() != 1) {
		throw(new compchem::SizeMismatchException());
	}


	qsort(this->data, this->getShape(0), sizeof(double), compare);
	//sort_work(this->data, 0, this->getSize() - 1);
}

template<>
compchem::FortranArray<double> &compchem::FortranArray<double>::transpose() {
	if(this->getShape().getDimensionality() > 2) {
		throw(new compchem::SizeMismatchException());
	}

	FortranArray<double> *out;
	if(this->getShape().getDimensionality() == 1) {
		out = new FortranArray<double>(Dimension(1, this->getShape(0)));
		for(int i = 0; i < this->getShape(0); i++) {
			(*out)(0, i) = (*this)(i);
		}
	} else {
		out = new FortranArray<double>(Dimension(this->getShape(1), this->getShape(0)));
		for(int i = 0; i < this->getShape(0); i++) {
			for(int j = 0; j < this->getShape(1); j++) {
				(*out)(j, i) = (*this)(i, j);
			}
		}
	}
	return (*out);
}

int compchem::eigenval_compute(bool compute_left_ev, bool compute_right_ev,
		compchem::FortranArray<double> const &input_array,
		compchem::FortranArray<std::complex<double>> *eigenval,
		compchem::FortranArray<double> *left_ev,
		compchem::FortranArray<double> *right_ev) {
	/*
	 * Make sure the input is square.
	 */
	if (input_array.getShape().getDimensionality() != 2
			|| input_array.getShape(0) != input_array.getShape(1)) {
		throw(new compchem::SizeMismatchException());
	}

	/*
	 * Define the pointers to pass to the function.
	 */

	double *hold = (double *) malloc(input_array.getSize() * sizeof(double));
	double *evalsr, *evalsi, *left, *right;

	for (int i = 0; i < input_array.getSize(); i++) {
		hold[i] = input_array.getData()[i];
	}

	evalsr = (double *) malloc(input_array.getShape(0) * sizeof(double));
	evalsi = (double *) malloc(input_array.getShape(0) * sizeof(double));
	if (compute_left_ev) {
		left = (double *) malloc(input_array.getShape(0) * sizeof(double));
	} else {
		left = NULL;
	}

	if (compute_right_ev) {
		right = (double *) malloc(input_array.getShape(0) * sizeof(double));
	} else {
		right = NULL;
	}

	int retval = LAPACKE_dgeev(LAPACK_ROW_MAJOR, (compute_left_ev) ? 'V' : 'N',
			(compute_right_ev) ? 'V' : 'N', input_array.getShape(0), hold,
			input_array.getShape(0), evalsr, evalsi, left,
			input_array.getShape(0), right, input_array.getShape(0));

	if (compute_left_ev && left_ev != nullptr) {
		for(int i = 0; i < left_ev->getSize(); i++) {
			left_ev->data[i] = left[i];
		}
	}
	free(left);
	if (compute_right_ev && right_ev != nullptr) {
		for(int i = 0; i < right_ev->getSize(); i++) {
			right_ev->data[i] = right[i];
		}
	}
	free(right);
	if (eigenval != nullptr) {
		for (int i = 0; i < input_array.getShape(0); i++) {
			eigenval->data[i] = std::complex<double>(evalsr[i], evalsi[i]);
		}
	}
	free(evalsr);
	free(evalsi);
	return (retval);
}

int compchem::eigenval_compute(bool compute_left_ev, bool compute_right_ev,
		compchem::FortranArray<double> const &input_array,
		compchem::FortranArray<double> *eigenvalr,
		compchem::FortranArray<double> *eigenvali,
		compchem::FortranArray<double> *left_ev,
		compchem::FortranArray<double> *right_ev) {
	/*
	 * Make sure the input is square.
	 */
	if (input_array.getShape().getDimensionality() != 2
			|| input_array.getShape(0) != input_array.getShape(1)) {
		throw(new compchem::SizeMismatchException());
	}

	/*
	 * Define the pointers to pass to the function.
	 */

	double *hold = (double *) malloc(input_array.getSize() * sizeof(double));
	double *evalsr, *evalsi, *left, *right;

	for (int i = 0; i < input_array.getSize(); i++) {
		hold[i] = input_array.getData()[i];
	}

	evalsr = (double *) malloc(input_array.getShape(0) * sizeof(double));
	evalsi = (double *) malloc(input_array.getShape(0) * sizeof(double));
	if (compute_left_ev) {
		left = (double *) malloc(input_array.getShape(0) * sizeof(double));
	} else {
		left = NULL;
	}

	if (compute_right_ev) {
		right = (double *) malloc(input_array.getShape(0) * sizeof(double));
	} else {
		right = NULL;
	}

	int retval = LAPACKE_dgeev(LAPACK_ROW_MAJOR, (compute_left_ev) ? 'V' : 'N',
			(compute_right_ev) ? 'V' : 'N', input_array.getShape(0), hold,
			input_array.getShape(0), evalsr, evalsi, left,
			input_array.getShape(0), right, input_array.getShape(0));

	if (compute_left_ev && left_ev != nullptr) {
		for(int i = 0; i < left_ev->getSize(); i++) {
			left_ev->data[i] = left[i];
		}
	}
	free(left);
	if (compute_right_ev && right_ev != nullptr) {
		for(int i = 0; i < right_ev->getSize(); i++) {
			right_ev->data[i] = right[i];
		}
	}
	free(right);
	if (eigenvalr != nullptr) {
		for(int i = 0; i < eigenvalr->getSize(); i++) {
			eigenvalr->data[i] = evalsr[i];
		}
	}
	if (eigenvali != nullptr) {
		for(int i = 0; i < eigenvali->getSize(); i++) {
			eigenvali->data[i] = evalsi[i];
		}
	}
	free(evalsr);
	free(evalsi);
	return (retval);
}

template<>
double &compchem::FortranArray<double>::subscript(int ind0, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) {
	int ind = 0;
	int size = 1;
	std::array<int, 7> inds = { ind0, ind1, ind2, ind3, ind4, ind5, ind6 };

	for (int i = 0; i < 7; i++) {
		if (inds[i] > dim[i] || inds[i] < 0) {
			throw(new ArrayIndexOutOfBoundsException);
		}
		if (inds[i] == 0) {
			break;
		}
		ind += (inds[i] - 1) * size;
		inds[i] *= dim[i];
	}
	return (data[ind]);
}

template<>
const double &compchem::FortranArray<double>::subscript(int ind0, int ind1, int ind2, int ind3,
			int ind4, int ind5, int ind6) const {
	int ind = 0;
	int size = 1;
	std::array<int, 7> inds = { ind0, ind1, ind2, ind3, ind4, ind5, ind6 };

	for (int i = 0; i < 7; i++) {
		if (inds[i] > dim[i] || inds[i] < 0) {
			throw(new ArrayIndexOutOfBoundsException);
		}
		if (inds[i] == 0) {
			break;
		}
		ind += (inds[i] - 1) * size;
		inds[i] *= dim[i];
	}
	return (data[ind]);
}
