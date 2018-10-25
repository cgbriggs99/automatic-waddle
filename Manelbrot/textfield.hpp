/*
 * textfield.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef TEXTFIELD_HPP_
#define TEXTFIELD_HPP_

#include "visible.hpp"
#include <string>
#include "window.hpp"
#include <string.h>
#include <stdlib.h>
#include "update_command.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>

class TextField : public Visible, public UpdateCommandObserver {
private:
	unsigned char *value;
	int x, y, width, height;
	void *stroke_font;
	color_t color;
public :
	TextField(void *stroke_font, char *val, int x, int y, color_t color) {
		this->value = (unsigned char *) calloc(strlen(val) + 1, sizeof(unsigned char));
		memcpy(this->value, val, strlen(val));
		this->width = glutBitmapLength(stroke_font, (unsigned char *) val);
		this->x = x;
		this->y = y;
		this->stroke_font = stroke_font;
		this->height = glutBitmapHeight(stroke_font);
		this->color = color;
	}
	~TextField() {
		free(this->value);
	}

	void draw() override {
		Window *w = Window::getSingleton();
		glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glutBitmapString(this->stroke_font, this->value);
		glPopMatrix();
	}

	const unsigned char *getValue() {
		return (this->value);
	}

	void setValue(const char *val) {
		this->value = (unsigned char *) realloc(this->value, (strlen(val) + 1) * sizeof(unsigned char));
		memcpy(this->value, val, strlen(val));
		this->value[strlen(val)] = 0;
		this->setUpdate(true);
	}
};

#endif /* TEXTFIELD_HPP_ */
