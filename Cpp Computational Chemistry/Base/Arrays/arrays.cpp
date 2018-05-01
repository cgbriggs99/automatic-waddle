/*
 * arrays.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */
#ifndef ARRAYS_CPP__
#define ARRAYS_CPP__
#include "../../arrays.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

using namespace array;

template<class T>
array::Array<T>::Array(int dim, ...) {
	va_list lst;
	va_start(lst, dim);

	this->dim = dim;
	this->sizes = (size_t *) calloc(dim, sizeof(size_t));
	this->total = 1;
	this->mults = (size_t *) calloc(dim, sizeof(size_t));
	this->mults[0] = 1;

	for(int i = 0; i < dim; i++) {
		this->sizes[i] = va_arg(lst, int);
		this->mults[i] = this->total;
		this->total *= this->sizes[i];
	}
	this->data = new T[this->total];
	this->freeOnDelete = true;
	va_end(lst);
}


template<class T>
array::Array<T>::Array(T *data, int dim, ...) {
	va_list lst;
	va_start(lst, dim);

	this->dim = dim;
	this->sizes = (size_t *) calloc(dim, sizeof(size_t));
	this->total = 1;
	this->mults = (size_t *) calloc(dim, sizeof(size_t));

	for(int i = 0; i < dim; i++) {
		this->sizes[i] = va_arg(lst, int);
		this->mults[i] = this->total;
		this->total *= this->sizes[i];
	}
	this->data = data;
	this->freeOnDelete = false;
	va_end(lst);
}

template<class T>
array::Array<T>::Array(const Array<T> &arr) {
	this->dim = arr.dim;
	this->total = arr.total;
	this->data = new T[this->total];
	this->sizes = (size_t *) calloc(this->dim, sizeof(size_t));
	this->mults = (size_t *) calloc(this->dim, sizeof(size_t));
	for(int i = 0; i < this->total; i++) {
		T temp = arr.data[i];
		this->data[i] = temp;
	}
	for(int i = 0; i < this->dim; i++) {
		this->sizes[i] = arr.sizes[i];
		this->mults[i] = arr.mults[i];
	}
	this->freeOnDelete = false;
}


template<class T>
array::Array<T>::~Array() {
	free(this->sizes);
	if(this->freeOnDelete) {
		delete [] this->data;
	}
}

template<class T>
array::Array<T> *array::Array<T>::toArray(T *data, int dim, ...) {
	va_list lst;
	va_start(lst, dim);

	array::Array<T> *out = new array::Array<T>(dim, lst);
	for(int i = 0; i < out->total; i++) {
		T temp = data[i];
		out->data[i] = temp;
	}
	out->freeOnDelete = true;
	va_end(lst);
}

template<class T>
T &array::Array<T>::operator()(int ind1, ...) {
	va_list lst;
	va_start(lst, ind1);

	size_t subsc = ind1 * this->mults[this->dim - 1];

	for(int i = this->dim - 2; i >= 0; i--) {
		int curr = va_arg(lst, int);
		if(curr >= this->sizes[i] || curr < 0) {
			throw array::ArrayIndexOutOfBoundsException();
		}
		subsc += this->mults[i] * curr;
	}

	va_end(lst);
	return (*&(this->data[subsc]));
}

#endif
