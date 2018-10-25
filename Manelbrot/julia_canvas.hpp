/*
 * julia_canvas.hpp
 *
 *  Created on: Oct 25, 2018
 *      Author: connor
 */

#ifndef JULIA_CANVAS_HPP_
#define JULIA_CANVAS_HPP_

#include "textfield.hpp"
#include "canvas.hpp"
#include "mandelbrot.hpp"

class JuliaCanvas : public Canvas, public MouseObserver {
protected:
	long double zoom;
	std::complex<long double> base, offset;
	int poll;
	TextField *click_pos;
public:
	JuliaCanvas(std::complex<long double> base, int x, int y, int width, int height, bool frame, TextField *click_pos) :
		Canvas(x, y, width, height, frame) {
			zoom = 0.5;
			offset = 0;
			poll = 100;
			this->click_pos = click_pos;
			this->base = base;
		}

		void paint() override {
			if(needs_update) {
				needs_update = false;
				for(int i = 0; i < this->getWidth(); i++) {
					for(int j = 0; j < this->getHeight(); j++) {
						std::complex<long double> c;
						c.real(((2.0 * i) / this->getWidth() - 1) / zoom);
						c.imag(((2.0 * j) / this->getHeight() - 1) / zoom);
						setPixel(i, j, mandelbrot( c + offset, base, this->poll));
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
			this->offset = std::complex<long double>(x, this->offset.imag());
			this->setUpdate(true);
		}

		void setY(long double y) {
			this->offset = std::complex<long double>(this->offset.real(), y);
			this->setUpdate(true);
		}

		void setPoll(int poll) {
			this->poll = poll;
			this->setUpdate(true);
		}

		void onClick(int button, int status, int x, int y) {
			if(x > this->getX() && x < this->getX() + this->getWidth() &&
					y > this->getY() && y < this->getY() + this->getHeight()) {
				char *str = (char *) calloc(100, sizeof(char));
				sprintf(str, "Last click: \n%.20Lf\n%.20Lf i", ((2.0F * (x - this->getX())) / (this->getWidth()) - 1) / this->zoom + this->offset.real(),
						(1 - (2.0F * (y - this->getY())) / (this->getHeight())) / this->zoom + this->offset.imag());
				this->click_pos->setValue(str);
				free(str);
				Renderer::getSingleton()->setUpdate(true);
			}
		}
};

#endif /* JULIA_CANVAS_HPP_ */
