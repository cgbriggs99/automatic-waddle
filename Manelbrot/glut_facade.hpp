/*
 * glut_facade.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef GLUT_FACADE_HPP_
#define GLUT_FACADE_HPP_

#include "window.hpp"
#include "focusable.hpp"
#include "idle.hpp"
#include "keyboard_commands.hpp"
#include "mouse_commands.hpp"
#include "renderer.hpp"
#include <GL/glut.h>

class GlutFacade {

	static GlutFacade *singleton;

	GlutFacade() {
		Renderer *Renderer::singleton = nullptr;
		Window *Window::singleton = nullptr;
		UpdateCommandReceiver *UpdateCommandReceiver::singleton = nullptr;
		IdleMediator *IdleMediator::singleton = nullptr;
		FocusMediator *FocusMediator::singleton = nullptr;
		KeyboardReceiver *KeyboardReceiver::singleton = nullptr;
		MouseReceiver *MouseReceiver::singleton = nullptr;
	}

	static void display(void) {
		Renderer *instance = instance->getSingleton();
		instance->render();
	}

	static void reshape(int wid, int hei) {
		if(wid != WIDTH || hei != HEIGHT || glutGet(GLUT_WINDOW_WIDTH) != WIDTH || glutGet(GLUT_WINDOW_HEIGHT) != HEIGHT) {
			Window::getSingleton()->resize(WIDTH, HEIGHT);
		}
		Renderer::getSingleton()->setUpdate(true);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	static void visibility(int status) {
		if(status == GLUT_VISIBLE) {
			Renderer::getSingleton()->setUpdate(true);
			display();
		}
	}

	static void idle(void) {
		IdleMediator::getSingleton()->loopIdle();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	static void keyboard(unsigned char key, int x, int y) {
		KeyboardReceiver::getSingleton()->update(key, x, y);
		Renderer::getSingleton()->setUpdate(true);
	}

	static void mouse(int button, int status, int x, int y) {
		MouseReceiver::getSingleton()->update(button, status, x, y);
	}
public:

	static GlutFacade *getSingleton() {
		if(GlutFacade::singleton == nullptr) {
			GlutFacade::singleton = new GlutFacade();
		}
		return (singleton);
	}

};

#endif /* GLUT_FACADE_HPP_ */
