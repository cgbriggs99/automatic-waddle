/*
 * color.cpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#include "defs.hpp"
#include <stdlib.h>
#include <math.h>

color_t makeColor4i(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	color_t out;
	out.a = a;
	out.b = b;
	out.g = g;
	out.r = r;
	return (out);
}

color_t makeColor4f(float r, float g, float b, float a) {
	color_t color;
	color.a = (unsigned char) (255 * a);
	color.b = (unsigned char) (255 * b);
	color.g = (unsigned char) (255 * g);
	color.r = (unsigned char) (255 * r);
	return (color);
}

color_t alphaComposite(color_t top, color_t bottom) {
	color_t out;
	float tr, tg, tb, ta, br, bg, bb, ba;
	tr = top.r / 255.0f;
	tg = top.g / 255.0f;
	tb = top.b / 255.0f;
	ta = top.a / 255.0f;
	br = bottom.r / 255.0f;
	bg = bottom.g / 255.0f;
	bb = bottom.b / 255.0f;
	ba = bottom.a / 255.0f;
	float r, g, b, a;
	r = (tr * ta + br * ba * (1 - ta)) / (ta + ba * (1 - ta));
	g = (tg * ta + bg * ba * (1 - ta)) / (ta + ba * (1 - ta));
	b = (tb * ta + bb * ba * (1 - ta)) / (ta + ba * (1 - ta));
	a = ta + ba * (1 - ta);
	out.r = (unsigned char) ((r > 1)? 255: 255 * r);
	out.b = (unsigned char) ((b > 1)? 255: 255 * b);
	out.g = (unsigned char) ((g > 1)? 255: 255 * g);
	out.a = (unsigned char) ((a > 1)? 255: 255 * a);
	return (out);
}

color_t hsvToRGB(float h /* From 0 to 1 */, float s, float v, float a) {
	float chroma = s * v;
	float hp = fmod(h, 1.0) * 6;
	float x = chroma * (1 - fabs(fmod(hp, 2) - 1));
	color_t out;
	float r, g, b;
	if(0 <= hp && hp <= 1) {
		r = chroma;
		g = x;
		b = 0;
	} else if(1 < hp && hp <= 2) {
		r = x;
		g = chroma;
		b = 0;
	} else if(2 < hp && hp <= 3) {
		r = 0;
		g = chroma;
		b = x;
	} else if(3 < hp && hp <= 4) {
		r = 0;
		g = x;
		b = chroma;
	} else if(4 < hp && hp <= 5) {
		r = x;
		g = 0;
		b = chroma;
	} else if(5 < hp && hp <= 6) {
		r = chroma;
		g = 0;
		b = x;
	}
	out.a = (unsigned char) (255 * a);
	out.b = (unsigned char) (255 * (b + v - chroma));
	out.g = (unsigned char) (255 * (g + v - chroma));
	out.r = (unsigned char) (255 * (r + v - chroma));
	return (out);
}
