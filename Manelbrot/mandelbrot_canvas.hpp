/*
 * mandelbrot_canvas.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef MANDELBROT_CANVAS_HPP_
#define MANDELBROT_CANVAS_HPP_

#include "./canvas.hpp"
#include "mandelbrot.hpp"
#include "mouse_commands.hpp"

class MandelbrotCanvas : public Canvas, public MouseObserver {
private:
	long double zoom;
	std::complex<long double> offset;
	int poll;
public:
	MandelbrotCanvas(int x, int y, int width, int height, bool frame) : Canvas(x, y, width, height, frame) {
		zoom = 0.5;
		offset = 0;
		poll = 100;
	}

	void paint() override {
		if(needs_update) {
			needs_update = false;
			for(int i = 0; i < this->getWidth(); i++) {
				for(int j = 0; j < this->getHeight(); j++) {
					std::complex<long double> c;
					c.real(((2.0 * i) / this->getWidth() - 1) / zoom);
					c.imag(((2.0 * j) / this->getHeight() - 1) / zoom);
					setPixel(i, j, mandelbrot(c + offset, this->poll));
				}
			}
			antialias();
		}
	}

	void setZoom(long double zoom) {
		this->zoom = zoom;
		this->setUpdate(true);
	}

	void setX(long double x) {
		this->offset = x;
		this->setUpdate(true);
	}

	void setY(long double y) {
		this->offset = std::complex<long double>(0, y);
		this->setUpdate(true);
	}

	void setPoll(int poll) {
		this->poll = poll;
		this->setUpdate(true);
	}
};

#endif /* MANDELBROT_CANVAS_HPP_ */
