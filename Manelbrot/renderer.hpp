/*
 * renderer.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "./visible.hpp"
#include "../Design Patterns/design_patterns.hpp"
#include <stdlib.h>
#include <GL/gl.h>
#include <C:/freeglut/include/GL/freeglut.h>

class Renderer {
private:
	Visible **to_draw;
	int len;
	int width, height;

	color_t *data;

	static Renderer *singleton = nullptr;
	Renderer() {
		to_draw = calloc(0, sizeof(Visible *));
		this->width = glutGet(GLUT_WINDOW_WIDTH);
		this->height = glutGet(GLUT_WINDOW_HEIGHT);
		this->len = 0;
		this->data = calloc(this->width * this->height, sizeof(color_t));
	}

	void compose() {
		for(int i = 0; i < len; i++) {
			Visible *curr = to_draw[i];
			color_t *color_buffer = curr->getVisibleData();
			for(int i = 0; i < curr->getWidth(); i++) {
				for(int j = 0; j < curr->getWidth(); j++) {
					data[curr->getX() + i + (curr->getY() + j) * width] =
							color_buffer[i + j * curr->getWidth()];
				}
			}
		}
	}
public :
	~Renderer() {
		for(int i = 0; i < len; i++) {
			delete(to_draw[i]);
		}
		free(to_draw);
	}

	static Renderer *getSingleton() {
		if(singleton == nullptr) {
			singleton = new Renderer();
		}
		return (singleton);
	}

	void registerVisible(Visible *v) {
		this->to_draw = realloc(this->to_draw, (1 + this->len) * sizeof(Visible *));
		this->to_draw[this->len] = v;
		this->len++;
	}

	void changeWindowSize(int new_width, int new_height) {
		this->data = realloc(this->data, new_width * new_height * sizeof(color_t));
	}

	void render() {
		this->compose();
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos2f(-1, 1);
		glDrawPixels(this->width, this->height, GL_RGBA, GL_INT, this->data);
		glFlush();
	}
};

#endif /* RENDERER_HPP_ */
