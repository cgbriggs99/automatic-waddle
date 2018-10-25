/*
 * defs.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef DEFS_HPP_
#define DEFS_HPP_

typedef struct {
	unsigned char a, b, g, r;
} color_t;

extern color_t makeColor4i(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
extern color_t makeColor4f(float r, float g, float b, float a);
extern color_t alphaComposite(color_t top, color_t bottom);
extern color_t hsvToRGB(float h /* From 0 to 1.0 */, float s, float v, float a);

#endif /* DEFS_HPP_ */
