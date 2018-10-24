/*
 * mandelbrot.cpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */


#include "./mandelbrot.hpp"
#include <complex.h>

using namespace std;
#define MAX_COUNT 10000

int mandelbrot(complex<long double> c) {
	complex<long double> z0, z1;
	z0 = 0;
	z1 = c;
	int count = 0;
	while(abs(z1) <= 2 && count < MAX_COUNT) {
		z0 = z1;
		z1 = z0 * z0 + c;
		count++;
	}

	return (count);
}

