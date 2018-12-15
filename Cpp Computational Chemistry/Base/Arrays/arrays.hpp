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
	Dimension() {
		shape = {0, 0, 0, 0, 0, 0, 0};
	}
	Dimension(int dim0, int dim1 = 0, int dim2 = 0, int dim3 = 0, int dim4 = 0,
	          int dim5 = 0, int dim6 = 0) {
		shape = {dim0, dim1, dim2, dim3, dim4, dim5, dim6};
	}
	~Dimension() = default;
	int operator[](int ind) const {
		return (shape[ind]);
	}
	Dimension &operator=(Dimension const &dim);
	friend bool operator==(Dimension dim1, Dimension dim2);
	friend bool operator!=(Dimension dim1, Dimension dim2);
	int getDimensionality();
};

bool operator==(Dimension dim1, Dimension dim2);
bool operator!=(Dimension dim1, Dimension dim2);

/*
 * For full FORTRAN feel, these arrays will be 1-indexed and use the () notation for indexing.
 * INTEGER, DIMENSION(
 */
template<typename _T>
class FortranArray {
private:
	_T *data;
	compchem::Dimension dim;

	static void __addition_thread(FortranArray<_T> *a, FortranArray<_T> *b,
	                              int id);
	static void __subtraction_thread(FortranArray<_T> *a, FortranArray<_T> *b,
	                                 int id);
	static void __multiplication_thread(FortranArray<_T> *a,
	                                    FortranArray<_T> *b, int id);
	static void __multiplication_thread(FortranArray<_T> *a, _T &b, int id);
	static void __division_thread(FortranArray<_T> *a, FortranArray<_T> *b,
	                              int id);
	static void __division_thread(FortranArray<_T> *a, _T &b, int id);
	static void __comparison_thread(FortranArray<_T> *a, FortranArray<_T> *b,
	                                int id, bool *conds);

public:
	explicit FortranArray(compchem::Dimension dim);
	explicit FortranArray(compchem::Dimension dim,
	                      std::initializer_list<_T> list);
	FortranArray(std::initializer_list<_T> list);
	explicit FortranArray(compchem::Dimension dim, _T *values);
	FortranArray(const FortranArray<_T> &arr);
	~FortranArray();

	compchem::Dimension getShape() const;
	int getShape(int ind) const;
	_T *getData() const;
	int getSize() const;

	_T &subscript(int ind0, int ind1 = 0, int ind2 = 0, int ind3 = 0, int ind4 =
	        0,
	              int ind5 = 0, int ind6 = 0);
	const _T &subscript(int ind0, int ind1 = 0, int ind2 = 0, int ind3 = 0,
	                    int ind4 = 0, int ind5 = 0, int ind6 = 0) const;
	_T &operator()(int ind0, int ind1 = 0, int ind2 = 0, int ind3 = 0,
	               int ind4 = 0, int ind5 = 0, int ind6 = 0);
	const _T &operator()(int ind0, int ind1 = 0, int ind2 = 0, int ind3 = 0,
	                     int ind4 = 0, int ind5 = 0, int ind6 = 0) const;

	FortranArray<_T> &operator=(const FortranArray<_T> &b);
	FortranArray<_T> &operator+=(const FortranArray<_T> &b);
	FortranArray<_T> &operator-=(const FortranArray<_T> &b);
	FortranArray<_T> &operator*=(const FortranArray<_T> &b);
	FortranArray<_T> &operator*=(const _T &b);
	FortranArray<_T> &operator/=(const FortranArray<_T> &b);
	FortranArray<_T> &operator/=(const _T &b);
	bool operator==(const FortranArray<_T> &b);

	template<typename _T1>
	friend FortranArray<_T1> operator+(FortranArray<_T1> &a,
	                                   const FortranArray<_T1> &b);

	template<typename _T1>
	friend compchem::FortranArray<_T1> operator-(compchem::FortranArray<_T1> a,
	                                   const compchem::FortranArray<_T1> &b);

	template<typename _T1>
	friend FortranArray<_T1> operator*(FortranArray<_T1> a, _T1 k);

	template<typename _T1>
	friend FortranArray<_T1> &operator*(_T1 k, FortranArray<_T1> a);

	//Standard Fortran * operator. Not matrix multiplication.
	template<typename _T1>
	friend FortranArray<_T1> operator*(FortranArray<_T1> a,
	                                   const FortranArray<_T1> &b);

	template<typename _T1>
	friend FortranArray<_T1> operator/(FortranArray<_T1> a,
	                                   const FortranArray<_T1> &b);

	template<typename _T1>
	friend FortranArray<_T1> operator/(FortranArray<_T1> a, _T1 k);

	_T dotprod(FortranArray<_T> &b);
	FortranArray<_T> &crossprod(FortranArray<_T> &b);

	template<typename _U>
	operator FortranArray<_U>();

//	template<typename _U>
//	explicit operator _U *();

	/*
	 * Friends that define useful connections to lapack and blas.
	 */
	template<typename _T1>
	friend FortranArray<_T1> &matmult(FortranArray<_T1> a1,
	                                  FortranArray<_T1> a2);

	friend FortranArray<double> &dgemm(bool transposea1, bool t_a2, int m,
	                                   int n, int k, double alpha,
	                                   FortranArray<double> a1,
	                                   FortranArray<double> a2, double beta,
	                                   FortranArray<double> c);
};

template<typename _T1>
FortranArray<_T1> operator+(FortranArray<_T1> &a,
                            const FortranArray<_T1> &b);

template<typename _T1>
FortranArray<_T1> operator-(FortranArray<_T1> a,
                            const FortranArray<_T1> &b);

template<typename _T1>
FortranArray<_T1> operator*(FortranArray<_T1> a, _T1 k);

template<typename _T1>
FortranArray<_T1> &operator*(_T1 k, FortranArray<_T1> a);

//Standard Fortran * operator. Not matrix multiplication.
template<typename _T1>
FortranArray<_T1> operator*(FortranArray<_T1> a,
                            const FortranArray<_T1> &b);

template<typename _T1>
FortranArray<_T1> operator/(FortranArray<_T1> a,
                            const FortranArray<_T1> &b);

template<typename _T1>
FortranArray<_T1> operator/(FortranArray<_T1> a, _T1 k);

/*
 * Friends that define useful connections to lapack and blas.
 */
template<typename _T1>
FortranArray<_T1> &matmult(FortranArray<_T1> a1,
	FortranArray<_T1> a2);

FortranArray<double> &dgemm(bool transposea1, bool t_a2, int m,
	int n, int k, double alpha,
	FortranArray<double> a1,
	FortranArray<double> a2, double beta,
	FortranArray<double> c);

}

#include "arrays.cpp"

#endif /* ARRAYS_HPP_ */
