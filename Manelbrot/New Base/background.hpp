/*
 * background.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include "visible.hpp"

#include <GL/gl.h>

class Background : public Visible {
private:
	float r, g, b, a;
public:
	Background(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		glClearColor(r, g, b, a);
	}

	~Background() {
		glClearColor(0, 0, 0, 0);
	}

	void draw() override {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

#endif /* BACKGROUND_HPP_ */
