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
#include <thread>
#include <vector>

class JuliaCanvas: public Canvas, public MouseObserver {
protected:
	long double zoom;
	std::complex<long double> base, offset;
	int poll;
	TextField *click_pos;
	int max_threads;
	bool working;
	std::vector<std::thread *> threads;

	virtual void computeMandelbrotThreaded(int id) {
		for (int i = id; i < this->getWidth(); i += this->max_threads) {
			for (int j = 0; j < this->getHeight(); j++) {
				std::complex<long double> c;
				c.real(((2.0 * i) / this->getWidth() - 1) / this->zoom);
				c.imag(((2.0 * j) / this->getHeight() - 1) / this->zoom);
				this->setPixel(i, j, mandelbrot(c + this->offset, this->base, this->poll));
			}
		}
	}

	void setWorking(bool working) {
		this->working = working;
	}
public:
	JuliaCanvas(std::complex<long double> base, int x, int y, int width,
			int height, bool frame, TextField *click_pos, int max_threads = 8) :
			Canvas(x, y, width, height, frame) {
		zoom = 0.5;
		offset = 0;
		poll = 100;
		this->click_pos = click_pos;
		this->base = base;
		this->max_threads = max_threads;
		this->working = false;
	}

	void paint() override {
		if (needs_update) {
			working = true;
			needs_update = false;
			std::vector<std::thread *> threads;
			for(int i = 0; i < max_threads; i++) {
				threads.push_back(new std::thread(computeMandelbrotThreaded, this, i));
			}
			for(int i = 0; i < max_threads; i++) {
				threads[i]->join();
			}
			this->working = false;

//			for (int i = 0; i < this->getWidth(); i++) {
//				for (int j = 0; j < this->getHeight(); j++) {
//					std::complex<long double> c;
//					c.real(((2.0 * i) / this->getWidth() - 1) / zoom);
//					c.imag(((2.0 * j) / this->getHeight() - 1) / zoom);
//					setPixel(i, j, mandelbrot(c + offset, base, this->poll));
//				}
//			}
			//antialias();
		}
	}

	void setZoom(long double zoom) {
		this->zoom = zoom;
		this->setUpdate(true);
	}

	void setOffsetX(long double x) {
		this->offset = std::complex<long double>(x, this->offset.imag());
		this->setUpdate(true);
	}

	void setOffsetY(long double y) {
		this->offset = std::complex<long double>(this->offset.real(), y);
		this->setUpdate(true);
	}

	void setPoll(int poll) {
		this->poll = poll;
		this->setUpdate(true);
	}

	long double getZoom() {
		return (this->zoom);
	}

	long double getOffsetX() {
		return (this->offset.real());
	}

	long double getOffsetY() {
		return (this->offset.imag());
	}

	int getPoll() {
		return (this->poll);
	}

	int getMaxThreads() {
		return (this->max_threads);
	}

	void setMaxThreads(int max_threads) {
		if(!working) {
			this->max_threads = max_threads;
		} else {
			throw "Can not change the number of threads while in use.";
		}
	}

	bool isWorking() {
		return (this->working);
	}

	void setOffset(std::complex<long double> off) {
		if(working) {
			throw "The program should not change important values while computing.";
		}
		this->offset = off;
	}

	std::complex<long double> getOffset() {
		return (this->offset);
	}

	std::vector<std::thread::id> getThreadIDs() {
		std::vector<std::thread::id> out;
		for(int i = 0; i < this->threads.size(); i++) {
			out.push_back(this->threads[i]->get_id());
		}
		return (out);
	}


	void onClick(int button, int status, int x, int y) {
		if (x > this->getX() && x < this->getX() + this->getWidth()
				&& y > this->getY() && y < this->getY() + this->getHeight()) {
			if (status == 0) {
				char *str = (char *) calloc(100, sizeof(char));
				sprintf(str, "Last click: \n%.20Lf\n%.20Lf i",
						((2.0F * (x - this->getX())) / (this->getWidth()) - 1)
								/ this->zoom + this->offset.real(),
						(1 - (2.0F * (y - this->getY())) / (this->getHeight()))
								/ this->zoom + this->offset.imag());
				this->click_pos->setValue(str);
				free(str);
				Renderer::getSingleton()->setUpdate(true);
			}
		}
	}
};

#endif /* JULIA_CANVAS_HPP_ */
