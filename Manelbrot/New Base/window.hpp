/*
 * window.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: cgbri
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "Design Patterns/design_patterns.hpp"
#include "command.hpp"
#include "glutcommands.hpp"
#include <GL/freeglut.h>

class Window : public design_patterns::Singleton<Window>, public Receiver {
private:
	int width, height;
	bool visible;
	bool mouse_is_in;

	Window() {
		this->width = glutGet(GLUT_WINDOW_WIDTH);
		this->height = glutGet(GLUT_WINDOW_HEIGHT);
		this->visible = true;
	}
public:
	int getWidth() {
		return (this->width);
	}

	int getHeight() {
		return (this->height);
	}

	bool isVisible() {
		return (this->visible);
	}

	bool hasMouse() {
		return (this->mouse_is_in);
	}

	void setWidth(int width) {
		this->width = width;
	}

	void setHeight(int height) {
		this->height = height;
	}

	void setVisibility(bool vis) {
		this->visible = vis;
	}

	void setHasMouse(bool mouse) {
		this->mouse_is_in = mouse;
	}

	void reshape(int width, int height) {
		this->width = width;
		this->height = height;
		glutReshapeWindow(width, height);
	}

	void receive(GlutReshapeCommand &command) override {
		this->setHeight(command.getHeight());
		this->setWidth(command.getWidth());
	}

	void receive(GlutVisibilityCommand &command) override {
		this->setVisibility(command.getState());
	}
};

#endif /* WINDOW_HPP_ */
