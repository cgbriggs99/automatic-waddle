/*
 * arrays.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: connor
 */



#ifndef __ARRAYS_CPP__
#define __ARRAYS_CPP__

#include "arrays.hpp"

template<typename _T>
compchem::Dimension const compchem::FortranArray<_T>::getShape();

template<typename _T>
int const compchem::FortranArray<_T>::getShape(int ind);

template<typename _T>
_T *const compchem::FortranArray<_T>::getData();

template<typename _T>
int const compchem::FortranArray<_T>::getSize();

template<typename _T>
compchem::FortranArray<_T>::FortranArray(compchem::Dimension dim) {
	this->dim = dim;
	this->data = new _T[this->getSize()];
}

template<typename _T>
compchem::FortranArray<_T>::FortranArray(compchem::Dimension dim, std::initializer_list<_T> list) {
	this->dim = dim;
	this->data = new _T[this->getSize()];
	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] = list.begin()[i];
	}
}

template<typename _T>
compchem::FortranArray<_T>::FortranArray(std::initializer_list<_T> list) {
	this->dim = list.size();
	this->data = new _T[this->getSize()];
	for(int i = 0; i < list.size(); i++) {
		this->data[i] = list.begin()[i];
	}
}



template<typename _T>
compchem::FortranArray<_T>::FortranArray(compchem::Dimension dim, _T *values) {
	this->dim = dim;
	this->data = new _T[this->getSize()];
	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] = values[i];
	}
}

template<typename _T>
compchem::FortranArray<_T>::FortranArray(const FortranArray<_T> &arr) {
	this->dim = arr.dim;
	this->data = new _T[this->getSize()];
	for(int i = 0; i < this->getSize(); i++) {
		//Copy.
//		new(&(this->data[i])) _T(&(arr.data[i]));
		this->data[i] = arr.data[i];
	}
}

template<typename _T>
compchem::FortranArray<_T>::~FortranArray() {
	for(int i = 0; i < this->getSize(); i++) {
		delete(&(this->data[i]));
	}
	delete[] (this->data);
}

template<typename _T>
compchem::Dimension compchem::FortranArray<_T>::getShape() const {
	return (this->dim);
}

template<typename _T>
_T *compchem::FortranArray<_T>::getData() const {
	return (this->data);
}

template<typename _T>
_T &compchem::FortranArray<_T>::operator()(int ind0, int ind1, int ind2,
		int ind3, int ind4, int ind5, int ind6) {
	int ind = 0;
	int size = 1;
	std::array<int, 7> inds = {ind0, ind1, ind2, ind3, ind4, ind5, ind6};

	for(int i = 0; i < 7; i++) {
		if(inds[i] > dim[i] || inds[i] < 0) {
			throw(new ArrayIndexOutOfBoundsException);
		}
		if(inds[i] == 0) {
			break;
		}
		ind += (inds[i] - 1) * size;
		inds[i] *= dim[i];
	}
	return ((data[ind]));
}

template<typename _T>
const _T &compchem::FortranArray<_T>::operator()(int ind0, int ind1, int ind2,
		int ind3, int ind4, int ind5, int ind6) const {
	int ind = 0;
	int size = 1;
	std::array<int, 7> inds = {ind0, ind1, ind2, ind3, ind4, ind5, ind6};

	for(int i = 0; i < 7; i++) {
		if(inds[i] > dim[i] || inds[i] < 0) {
			throw(new ArrayIndexOutOfBoundsException);
		}
		if(inds[i] == 0) {
			break;
		}
		ind += (inds[i] - 1) * size;
		inds[i] *= dim[i];
	}
	return (data[ind]);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator=(const FortranArray<_T> &b) {
	delete[] this->data;

	this->dim = b.dim;
	this->data = new _T[b.getSize()];

	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] = b.data[i];
	}

	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator+=(const FortranArray<_T> &b) {
	if(this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] += b.data[i];
	}

	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator-=(const FortranArray<_T> &b) {
	if(this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] -= b.data[i];
	}

	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator*=(const FortranArray<_T> &b) {
	if(this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] *= b.data[i];
	}

	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator*=(const _T &b) {
	if(this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] *= b;
	}

	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator/=(const _T &b) {
	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] /= b;
	}

	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator/=(const FortranArray<_T> &b) {
	if(this->getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for(int i = 0; i < this->getSize(); i++) {
		this->data[i] /= b.data[i];
	}
	return (*this);
}

template<typename _T>
compchem::FortranArray<_T> compchem::operator+(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(a.dim);

	if(a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	for(int i = 0; i < a.getSize(); i++) {
		out = a.data[i] + b.data[i];
	}

	return (*out);
}

template<typename _T>
compchem::FortranArray<_T> compchem::operator-(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {

	if(a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}
	compchem::FortranArray<_T> out(a.getShape());

	for(int i = 0; i < a.getSize(); i++) {
		out.data[i] = a.data[i] - b.data[i];
	}

	return (out);
}

template<typename _T>
compchem::FortranArray<_T> compchem::operator*(compchem::FortranArray<_T> a, _T k) {
	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(a.dim);

	for(int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] * k;
	}

	return (*out);
}

template<typename _T>
compchem::FortranArray<_T> compchem::operator*(_T k, compchem::FortranArray<_T> a) {
	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(a.dim);

	for(int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] * k;
	}

	return (*out);
}

//Standard Fortran * operator. Not matrix multiplication.
template<typename _T>
compchem::FortranArray<_T> compchem::operator*(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	if(a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(a.dim);

	for(int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] * b.data[i];
	}

	return (*out);
}

template<typename _T>
compchem::FortranArray<_T> compchem::operator/(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	if(a.getShape() != b.getShape()) {
		throw(new compchem::SizeMismatchException());
	}

	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(a.dim);

	for(int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] / b.data[i];
	}

	return (*out);
}

template<typename _T>
compchem::FortranArray<_T> compchem::operator/(compchem::FortranArray<_T> a, _T k) {
	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(a.dim);

	for(int i = 0; i < a.getSize(); i++) {
		out->data[i] = a.data[i] / k;
	}

	return (*out);
}

template<typename _T>
template<typename _U>
compchem::FortranArray<_T>::operator compchem::FortranArray<_U>() {
	compchem::FortranArray<_U> out(this->dim);
	for(int i = 0; i < this->getSize(); i++) {
		out.data[i] = (_U) this->data[i];
	}
	return (out);
}

/*
 * Friends that define useful connections to lapack and blas.
 */
template<typename _T>
compchem::FortranArray<_T> &compchem::matmult(compchem::FortranArray<_T> a1, compchem::FortranArray<_T> a2) {
	//Use the basic algorithm at first. Later, may speed it up with CUDA.
	if(a1.getShape().getDimensionality() != 2 || a2.getShape().getDimensionality() != 2) {
		throw(new compchem::SizeMismatchException);
	}
	if(a1.getShape()[0] != a2.getShape()[1]) {
		throw(new compchem::SizeMismatchException);
	}

	compchem::FortranArray<_T> *out = new compchem::FortranArray<_T>(
			compchem::Dimension(a2.getShape()[0], a1.getShape()[1]));

	for(int i = 0; i < a1.getShape()[1]; i++) {
		for(int j = 0; j < a2.getShape()[0]; j++) {
			(*out)(j, i) = (_T) 0;
			for(int k = 0; k < a1.getShape()[0]; k++) {
				(*out)(j, i) += a1(k, i) * a2(j, k);
			}
		}
	}
	return (out);
}

template<typename _T>
_T compchem::FortranArray<_T>::dotprod(FortranArray<_T> &b) {
	if(this->getShape(1) != b.getShape(1)) {
		throw(new compchem::SizeMismatchException);
	}
	_T accum = (*this)(0) * b(0);
	for(int i = 1; i < this->getShape(1); i++) {
		accum += (*this)(i) * b(i);
	}
	return (accum);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::crossprod(
	compchem::FortranArray<_T> &b) {
	if(this->getShape(1) != 3 || b.getShape(1) != 3) {
		throw(new compchem::CrossProductException);
	}
	compchem::FortranArray<_T> *out = new FortranArray<_T>(Dimension(3));
	(*out)(0) = (*this)(1) * b(2) - (*this)(2) * b(1);
	(*out)(1) = (*this)(0) * b(2) - (*this)(2) * b(0);
	(*out)(2) = (*this)(0) * b(1) - (*this)(1) * b(0);

	return (*out);
}

#endif
