/*
 * exceptions.hpp
 *
 *  Created on: Nov 26, 2018
 *      Author: connor
 */

#ifndef BASE_ARRAYS_EXCEPTIONS_HPP_
#define BASE_ARRAYS_EXCEPTIONS_HPP_

#include <exception>

namespace compchem {

struct ArrayIndexOutOfBoundsException : std::exception {
	const char *what() const throw() {
		return ("Array index out of bounds.");
	}
};

struct CrossProductException : std::exception {
	const char *what() const throw() {
		return ("Invalid size to cross product.");
	}
};

struct SizeMismatchException : std::exception {
	const char *what() const throw() {
		return ("Size mismatch between arguments.");
	}
};
}

#endif /* BASE_ARRAYS_EXCEPTIONS_HPP_ */
