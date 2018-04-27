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

template<class T>
class Array {
private:
	size_t dim;
	size_t total;
	bool freeOnDelete;
	size_t *sizes;
	T *data;
public:
	Array(int dim, ...);
	Array(T *data, int dim, ...);	//Does not copy. Simply sets the data pointer.
	Array(Array<T> &arr);
	~Array();

	static Array<T> *toArray(T *data, int dim, ...);	//Does copy.

	T *operator()(int ind1, ...);

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

}


#endif /* ARRAYS_HPP_ */
