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
#include "textfield.hpp"
#include "julia_canvas.hpp"

class MandelbrotCanvas : public JuliaCanvas {
private:
//	long double zoom;
//	std::complex<long double> offset;
//	int poll;
//	TextField *click_pos;
public:
	MandelbrotCanvas(int x, int y, int width, int height, bool frame, TextField *click_pos) :
		JuliaCanvas(std::complex<long double>(0), x, y, width, height, frame, click_pos) {
//		zoom = 0.5;
//		offset = 0;
//		poll = 100;
//		this->click_pos = click_pos;
	}

	void paint() override {
		if(needs_update) {
			needs_update = false;
			for(int i = 0; i < this->getWidth(); i++) {
				for(int j = 0; j < this->getHeight(); j++) {
					std::complex<long double> c;
					c.real(((2.0F * i) / this->getWidth() - 1) / zoom);
					c.imag(((2.0F * j) / this->getHeight() - 1) / zoom);
					setPixel(i, j, mandelbrot(std::complex<long double>(0), c + offset, this->poll));
				}
			}
			//antialias();
		}
	}
//
//	void setZoom(long double zoom) {
//		this->zoom = zoom;
//		this->setUpdate(true);
//	}
//
//	void setX(long double x) {
//		this->offset = std::complex<long double>(x, this->offset.imag());
//		this->setUpdate(true);
//	}
//
//	void setY(long double y) {
//		this->offset = std::complex<long double>(this->offset.real(), y);
//		this->setUpdate(true);
//	}
//
//	void setPoll(int poll) {
//		this->poll = poll;
//		this->setUpdate(true);
//	}
//
//	void onClick(int button, int status, int x, int y) {
//		if(x > this->getX() && x < this->getX() + this->getWidth() &&
//				y > this->getY() && y < this->getY() + this->getHeight()) {
//			char *str = (char *) calloc(100, sizeof(char));
//			sprintf(str, "Last click: %Lf + %Lf i", ((2.0 * (x - this->getX())) / (this->getWidth()) - 1) / this->zoom + this->offset.real(),
//					(1 - (2.0 * (y - this->getY())) / (this->getHeight())) / this->zoom + this->offset.imag());
//			this->click_pos->setValue(str);
//			free(str);
//			Renderer::getSingleton()->setUpdate(true);
//		}
//	}
};

#endif /* MANDELBROT_CANVAS_HPP_ */
