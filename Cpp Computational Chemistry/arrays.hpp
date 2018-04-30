/*
 * arrays.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef ARRAYS_HPP_
#define ARRAYS_HPP_

#include <stdlib.h>

namespace array {

#include <exception>
class ArrayIndexOutOfBoundsException : std::exception {
	const char *what() const throw() {
		return ("Array index out of bounds.");
	}
};

class CrossProductException : std::exception {
	const char *what() const throw() {
		return ("Invalid size to cross product.");
	}
};

class SizeMismatchException : std::exception {
	const char *what() const throw() {
		return ("Size mismatch between arguments.");
	}
};

template<class T>
class Array {
protected:
	size_t dim;
	size_t total;
	bool freeOnDelete;
	size_t *sizes;
	T *data;
public:
	Array(int dim, ...);
	Array(T *data, int dim, ...);	//Does not copy. Simply sets the data pointer.
	Array(const Array<T> &arr);	//Does copy.
	~Array();

	static Array<T> *toArray(T *data, int dim, ...);	//Does copy.

	T &operator()(int ind1, ...);
	//const T &operator()(int ind1, ...) const;

	T *getArray() {
		return (this->data);
	}

	size_t getSize(int dim) {
		return (this->sizes[dim]);
	}

	size_t getTotalSize() {
		return (this->total);
	}

	size_t getDimensions() {
		return (this->dim);
	}
};

template<class T>
class Vector {
private:
	size_t size;
	T *data;
public:
	Vector(int size) {
		this->data = new T[size];
		this->size = size;
	}

	Vector(const T *data, int size) {
		this->data = new T[size];
		this->size = size;
		for(int i = 0; i < this->size; i++) {
			T temp = data[i];
			this->data[i] = temp;
		}
	}

	Vector(const Vector<T> &vec) {
		this->data = new T[vec.size];
		this->size = vec.size;
		for(int i = 0; i < this->size; i++) {
			T temp = vec.data[i];
			this->data[i] = temp;
		}
	}

	~Vector() {
		delete [] this->data;
	}

	T &operator[](int ind) {
		return this->data[ind];
	}

	const T &operator[](int ind) const {
		return this->data[ind];
	}

	friend Vector<T> operator+(Vector<T> a, const Vector<T> &b) {
		return (a += b);
	}

	friend Vector<T> operator-(Vector<T> a, const Vector<T> &b) {
		return (a -= b);
	}

	friend Vector<T> operator%(Vector<T> a, const Vector<T> &b) {
		if(a.size != 3 || b.size != 3) {
			throw CrossProductException();
		}
		T x, y, z;
		x = a.data[1] * b.data[2] - a.data[2] * b.data[1];
		y = a.data[0] * b.data[2] - a.data[2] * b.data[0];
		z = a.data[0] * b.data[1] - a.data[1] * b.data[0];
		a.data[0] = x;
		a.data[1] = y;
		a.data[2] = z;
		return (a);
	}

	friend T operator*(Vector<T> a, const Vector<T> &b) {
		if(a.size != b.size) {
			throw SizeMismatchException();
		}
		T out = a.data[0] * b.data[0];
		for(int i = 1; i < a.size; i++) {
			out += a.data[i] * b.data[1];
		}
		return (out);
	}

	Vector<T> operator+=(const Vector<T> &b) {
		if(this->size != b.size) {
			throw SizeMismatchException();
		}
		for(int i = 0; i < this->size; i++) {
			this->data[i] += b.data[i];
		}
		return (*this);
	}

	Vector<T> operator-=(const Vector<T> &b) {
		if(this->size != b.size) {
			throw SizeMismatchException();
		}
		for(int i = 0; i < this->size; i++) {
			this->data[i] -= b.data[i];
		}
		return (*this);
	}

	Vector<T> operator%=(const Vector<T> &b) {
		if(this->size != 3 || b.size != 3) {
			throw CrossProductException();
		}
		T x, y, z;
		x = this->data[1] * b.data[2] - this->data[2] * b.data[1];
		y = this->data[0] * b.data[2] - this->data[2] * b.data[0];
		z = this->data[0] * b.data[1] - this->data[1] * b.data[0];
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
		return (*this);
	}

	friend Vector<T> operator*(Vector<T> vec, const T &scal) {
		for(int i = 0; i < vec.size; i++) {
			vec.data[i] *= scal;
		}
		return (vec);
	}

	friend Vector<T> operator/(Vector<T> vec, const T &scal) {
		for(int i = 0; i < vec.size; i++) {
			vec.data[i] /= scal;
		}
		return (vec);
	}

	Vector<T> operator*=(const T &scal) {
		for(int i = 0; i < this->size; i++) {
			this->data[i] *= scal;
		}
		return (*this);
	}

	Vector<T> operator/=(const T &scal) {
		for(int i = 0; i < this->size; i++) {
			this->data[i] /= scal;
		}
		return (*this);
	}
};

}


#endif /* ARRAYS_HPP_ */
