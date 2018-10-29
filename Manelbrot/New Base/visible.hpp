/*
 * visible.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: cgbri
 */

#ifndef VISIBLE_HPP_
#define VISIBLE_HPP_

#include <GL/gl.h>
#include <GL/freeglut.h>

class Visible {
public:
	virtual ~Visible() = 0;

	virtual void draw() = 0;
};

#endif /* VISIBLE_HPP_ */
