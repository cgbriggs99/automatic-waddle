/*
 * canvas.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include "./defs.hpp"
#include "./visible.hpp"

class Canvas : Visible {
private :
	color_t *pixels;
public:
	Canvas(int x, int y, int width, int height) : Visible(x, y, width, height) {
		this->pixels = new color_t[width * height];
	}

	const color_t *getVisibleData() override {
		return (this->pixels);
	}

	void setPixel(int x, int y, color_t color) {
		pixels[x + y * width] = color;
	}

	void clear() {
		for(int i = 0; i < width * height; i++) {
			pixels[i] = {0, 0, 0, 0};
		}
	}

	void setColor(color_t color) {
		for(int i = 0; i < width * height; i++) {
			pixels[i] = color;
		}
	}

	void drawRectangle(int relx, int rely, int width, int height, color_t color) {
		for(int i = 0; i < width; i++) {
			pixels[relx + i + rely * this->width] = color;
			pixels[relx + i + (rely + height - 1) * this->width] = color;
		}

		for(int i = 0; i < height; i++) {
			pixels[relx + (rely + i) * this->width] = color;
			pixels[relx + width - 1 + (rely + i) * this->width] = color;
		}
	}
};

#endif /* CANVAS_HPP_ */
