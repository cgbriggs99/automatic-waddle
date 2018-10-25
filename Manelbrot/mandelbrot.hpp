/*
 * mandelbrot.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include "defs.hpp"
#include <complex>

extern color_t mandelbrot(std::complex<long double> z, std::complex<long double> c, int max_count);

#endif /* MANDELBROT_HPP_ */
