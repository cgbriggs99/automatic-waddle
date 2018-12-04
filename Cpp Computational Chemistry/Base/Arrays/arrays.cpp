/*
 * arrays.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: connor
 */

#include "arrays.hpp"



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
		this->data[i] = list[i];
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
		new(&(this->data[i])) _T(&(arr.data[i]));
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
_T &compchem::FortranArray<_T>::operator()(int ind0, int ind1 = 0, int ind2 = 0,
		int ind3 = 0, int ind4 = 0, int ind5 = 0, int ind6 = 0) {
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
	return (&(data[ind]));
}

template<typename _T>
const _T &compchem::FortranArray<_T>::operator()(int ind0, int ind1 = 0, int ind2 = 0,
		int ind3 = 0, int ind4 = 0, int ind5 = 0, int ind6 = 0) const {
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
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator+=(const FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread(FortranArray<_T>::__addition_thread, this, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator-=(const FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread(FortranArray<_T>::__subtraction_thread, this, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator*=(const FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(FortranArray<_T> *, FortranArray<_T> *, int))
				FortranArray<_T>::__multiplication_thread, this, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator*=(const _T &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(FortranArray<_T> *, _T &, int))
				FortranArray<_T>::__multiplication_thread, this, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (this);
}

template<typename _T>
compchem::FortranArray<_T> &compchem::FortranArray<_T>::operator/=(const _T &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(FortranArray<_T> *, _T &, int))
				FortranArray<_T>::__division_thread, this, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (this);
}

template<typename _T>
compchem::FortranArray<_T> operator+(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread(compchem::FortranArray<_T>::__addition_thread, &out, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
}

template<typename _T>
compchem::FortranArray<_T> operator-(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread(compchem::FortranArray<_T>::__subtraction_thread, &out, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
}

template<typename _T>
compchem::FortranArray<_T> operator*(compchem::FortranArray<_T> a, _T k) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(compchem::FortranArray<_T> *, _T &, int))
				compchem::FortranArray<_T>::__multiplication_thread, &out, k, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
}

template<typename _T>
compchem::FortranArray<_T> operator*(_T k, compchem::FortranArray<_T> a) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(compchem::FortranArray<_T> *, _T &, int))
				compchem::FortranArray<_T>::__multiplication_thread, &out, k, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
}

//Standard Fortran * operator. Not matrix multiplication.
template<typename _T>
compchem::FortranArray<_T> operator*(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(compchem::FortranArray<_T> *, compchem::FortranArray<_T> *, int))
				compchem::FortranArray<_T>::__multiplication_thread, &out, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
}

template<typename _T>
compchem::FortranArray<_T> operator/(compchem::FortranArray<_T> a, const compchem::FortranArray<_T> &b) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(compchem::FortranArray<_T> *, compchem::FortranArray<_T> *, int))
				compchem::FortranArray<_T>::__division_thread, &out, b, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
}

template<typename _T>
compchem::FortranArray<_T> operator/(compchem::FortranArray<_T> a, _T k) {
	std::thread **threads = (std::thread **) malloc(sizeof(void *) * __THREAD_COUNT__);

	compchem::FortranArray<_T> out(a);

	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i] = new std::thread((void (*)(compchem::FortranArray<_T> *, _T &, int))
				compchem::FortranArray<_T>::__division_thread, &out, k, i);
	}
	for(int i = 0; i < __THREAD_COUNT__; i++) {
		threads[i]->join();
		delete(threads[i]);
	}
	free(threads);

	return (out);
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

template<typename _T>
template<typename _U>
explicit compchem::FortranArray<_T>::operator _U *() {
	return (this->data);
}

/*
 * Friends that define useful connections to lapack and blas.
 */
template<typename _T>
compchem::FortranArray<_T> &matmult(compchem::FortranArray<_T> a1, compchem::FortranArray<_T> a2) {
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

