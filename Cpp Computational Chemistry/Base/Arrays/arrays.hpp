/*
 * arrays.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef ARRAYS_HPP_
#define ARRAYS_HPP_

#include <stdlib.h>
#include <exception>
#include "exceptions.hpp"
#include <array>
#include <initializer_list>
#include <thread>
#include <mutex>

#ifndef __THREAD_COUNT__
#define __THREAD_COUNT__ 8
#endif

namespace compchem {

class Dimension {
private:
	std::array<int, 7> shape;
public:
	constexpr Dimension(int dim0, int dim1 = 0, int dim2 = 0, int dim3 = 0, int dim4 = 0, int dim5 = 0, int dim6 = 0);
	~Dimension() = default;
	int operator[](int ind) const;
	friend bool operator==(Dimension dim1, Dimension dim2) const;
	int getDimensionality();
};

/*
 * For full FORTRAN feel, these arrays will be 1-indexed and use the () notation for indexing.
 * INTEGER, DIMENSION(
 */
template<typename _T>
class FortranArray {
private:
	_T *data;
	const compchem::Dimension dim;

	static void __addition_thread(FortranArray<_T> *a, FortranArray<_T> *b, int id);
	static void __subtraction_thread(FortranArray<_T> *a, FortranArray<_T> *b, int id);
	static void __multiplication_thread(FortranArray<_T> *a, FortranArray<_T> *b, int id);
	static void __multiplication_thread(FortranArray<_T> *a, _T &b, int id);
	static void __division_thread(FortranArray<_T> *a, FortranArray<_T> *b, int id);
	static void __division_thread(FortranArray<_T> *a, _T &b, int id);
	static void __comparison_thread(FortranArray<_T> *a, FortranArray<_T> *b, int id, bool *conds);

public:
	FortranArray(compchem::Dimension dim);
	explicit FortranArray(compchem::Dimension dim, std::initializer_list<_T> list);
	explicit FortranArray(compchem::Dimension dim, _T *values);
	FortranArray(const FortranArray<_T> &arr);
	~FortranArray();

	compchem::Dimension getShape() const;
	_T *getData() const;
	int getSize() const;

	_T &operator()(int ind0, int ind1 = 0, int ind2 = 0, int ind3 = 0, int ind4 = 0, int ind5 = 0, int ind6 = 0);
	const _T &operator()(int ind0, int ind1 = 0, int ind2 = 0, int ind3 = 0, int ind4 = 0, int ind5 = 0, int ind6 = 0) const;

	FortranArray<_T> &operator+=(const FortranArray<_T> &b);
	FortranArray<_T> &operator-=(const FortranArray<_T> &b);
	FortranArray<_T> &operator*=(const FortranArray<_T> &b);
	FortranArray<_T> &operator*=(const _T &b);
	FortranArray<_T> &operator/=(const FortranArray<_T> &b);
	FortranArray<_T> &operator/=(const _T &b);
	bool operator==(const FortranArray<_T> &b);

	template<typename _T>
	friend FortranArray<_T> operator+(FortranArray<_T> a, const FortranArray<_T> &b);

	template<typename _T>
	friend FortranArray<_T> operator-(FortranArray<_T> a, const FortranArray<_T> &b);

	template<typename _T>
	friend FortranArray<_T> operator*(FortranArray<_T> a, _T k);

	template<typename _T>
	friend FortranArray<_T> operator*(_T k, FortranArray<_T> a);

	//Standard Fortran * operator. Not matrix multiplication.
	template<typename _T>
	friend FortranArray<_T> operator*(FortranArray<_T> a, const FortranArray<_T> &b);

	template<typename _T>
	friend FortranArray<_T> operator/(FortranArray<_T> a, const FortranArray<_T> &b);

	template<typename _T>
	friend FortranArray<_T> operator/(FortranArray<_T> a, _T k);

	template<typename _U>
	operator FortranArray<_U> ();

	template<typename _U>
	explicit operator _U *();

	/*
	 * Friends that define useful connections to lapack and blas.
	 */
	template<typename _T>
	friend FortranArray<_T> &matmult(FortranArray<_T> a1, FortranArray<_T> a2);

	friend FortranArray<double> &dgemm(bool transposea1, bool t_a2, int m, int n, int k, double alpha,
			FortranArray<double> a1, FortranArray<double> a2, double beta, FortranArray<double> c);
};

#include "dimension.cpp"
#include "arrays.cpp"

}

#endif /* ARRAYS_HPP_ */
