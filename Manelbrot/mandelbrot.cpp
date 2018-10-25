/*
 * mandelbrot.cpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */


#include "./mandelbrot.hpp"
#include <complex.h>

using namespace std;

color_t mandelbrot(complex<long double> c, int max_count) {
	complex<long double> z0, z1;
	z0 = 0;
	z1 = c;
	int count = 0;
	while(abs(z1) <= 2 && count < max_count) {
		z0 = z1;
		z1 = z0 * z0 + c;
		count++;
	}

	if(count == max_count) {
		return (makeColor4i(0, 0, 0, 255));
	}
	if(count == 0) {
		return (makeColor4i(255, 255, 255, 255));
	}
	return (hsvToRGB(1.0 / count, 1, 1, 1));
}

