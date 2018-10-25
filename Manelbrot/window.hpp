/*
 * window.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <stdlib.h>
#include <GL/glut.h>

class Window {
private:
	static Window *singleton;

	int width;
	int height;

	Window() {
		this->width = glutGet(GLUT_WINDOW_WIDTH);
		this->height = glutGet(GLUT_WINDOW_HEIGHT);
	}

	void resize_command(int width, int height) {
		this->width = width;
		this->height = height;
	}
public:
	static Window *getSingleton() {
		if(Window::singleton == nullptr) {
			Window::singleton = new Window();
		}
		return (singleton);
	}

	int getWidth() {
		return (this->width);
	}

	int getHeight() {
		return (this->height);
	}

	void resize(int width, int height) {
		glutReshapeWindow(width, height);
		this->width = width;
		this->height = height;
		glutSwapBuffers();
	}
};

#endif /* WINDOW_HPP_ */
