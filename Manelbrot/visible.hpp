/*
 * visible.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef VISIBLE_HPP_
#define VISIBLE_HPP_

#include "./defs.hpp"
#include "./command.hpp"

class Visible {
protected:
	int x, y;
	int width, height;
public:
	Visible(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	virtual ~Visible();

	virtual const color_t *getVisibleData();



	int getX() {
		return (x);
	}

	int getY() {
		return (y);
	}

	int getWidth() {
		return (width);
	}

	int getHeight() {
		return (height);
	}

	void setX(int x) {
		this->x = x;
	}

	void setY(int y) {
		this->y = y;
	}

	void setWidth(int width) {
		this->width = width;
	}

	void setHeight(int height) {
		this->height = height;
	}
};

#endif /* VISIBLE_HPP_ */
