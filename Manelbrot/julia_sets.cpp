/*
 * julia_sets.cpp
 *
 *  Created on: Oct 25, 2018
 *      Author: connor
 */

#include "./renderer.hpp"
#include "./mandelbrot.hpp"
#include "background.hpp"
#include "mandelbrot_canvas.hpp"
#include "window.hpp"
#include "textfield.hpp"
#include "idle.hpp"
#include "focusable.hpp"
#include "keyboard_commands.hpp"
#include "mouse_commands.hpp"
#include "textinputfield.hpp"
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
//#include <C:/freeglut/include/GL/freeglut.h>

Renderer *Renderer::singleton = nullptr;
Window *Window::singleton = nullptr;
UpdateCommandReceiver *UpdateCommandReceiver::singleton = nullptr;
IdleMediator *IdleMediator::singleton = nullptr;
FocusMediator *FocusMediator::singleton = nullptr;
KeyboardReceiver *KeyboardReceiver::singleton = nullptr;
MouseReceiver *MouseReceiver::singleton = nullptr;

void display(void) {
	Renderer *instance = instance->getSingleton();
	instance->render();
}

void reshape(int wid, int hei) {
	if (wid != 800 || hei != 500 || glutGet(GLUT_WINDOW_WIDTH) != 800
			|| glutGet(GLUT_WINDOW_HEIGHT) != 500) {
		Window::getSingleton()->resize(800, 500);
	}
	Renderer::getSingleton()->setUpdate(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void visibility(int status) {
	if (status == GLUT_VISIBLE) {
		Renderer::getSingleton()->setUpdate(true);
		display();
	}
}

void idle(void) {
	IdleMediator::getSingleton()->loopIdle();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void keyboard(unsigned char key, int x, int y) {
	KeyboardReceiver::getSingleton()->update(key, x, y);
	Renderer::getSingleton()->setUpdate(true);
}

void mouse(int button, int status, int x, int y) {
	MouseReceiver::getSingleton()->update(button, status, x, y);
}

void text1(const unsigned char *str, void *mandel) {
	MandelbrotCanvas *c = (MandelbrotCanvas *) mandel;
	long double scale;
	sscanf((char *) str, "%Lf", &scale);
	c->setZoom(scale);
}

void text2(const unsigned char *str, void *mandel) {
	MandelbrotCanvas *c = (MandelbrotCanvas *) mandel;
	long double scale;
	sscanf((char *) str, "%Lf", &scale);
	c->setX(scale);
}

void text3(const unsigned char *str, void *mandel) {
	MandelbrotCanvas *c = (MandelbrotCanvas *) mandel;
	long double scale;
	sscanf((char *) str, "%Lf", &scale);
	c->setY(scale);
}

void text4(const unsigned char *str, void *mandel) {
	MandelbrotCanvas *c = (MandelbrotCanvas *) mandel;
	int scale;
	sscanf((char *) str, "%d", &scale);
	c->setPoll(scale);
}

int main(int argc, char **argv) {
	std::complex<long double> pos;
	char *str = (char *) calloc(100, sizeof(char));
	if(argc != 3) {
		time_t t = time(NULL);
		struct tm *date = localtime(&t);

		srand((int) (365.25 * date->tm_year) + date->tm_yday);

		long r = random(), theta = random();
		pos = std::complex<long double>(2 * (long double) r / RAND_MAX * cosl(theta),
				2 * (long double) r / RAND_MAX * sinl(theta));
		sprintf(str, "Julia Set of the Day: %Lf + %Lf i", pos.real(), pos.imag());
	} else {
		long double x, y;
		sscanf(argv[1], "%Lf", &x);
		sscanf(argv[2], "%Lf", &y);
		pos = std::complex<long double>(x, y);
		sprintf(str, "Julia Set %Lf + %Lf i", pos.real(), pos.imag());
	}
	glutInit(&argc, argv);
	glutInitWindowSize(800, 500);
	glutCreateWindow(str);
	glutShowWindow();

	glEnable(GL_NEVER);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ACCUM);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutVisibilityFunc(visibility);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);


	TextField *click_pos = new TextField(GLUT_BITMAP_HELVETICA_18,
			(char *) "Last click: ", 525, 100, makeColor4i(0, 0, 0, 255));
	JuliaCanvas *canvas = new JuliaCanvas(pos, 0, 0, 500, 500, true,
			click_pos);

	Renderer::getSingleton()->registerVisible(new Background(1, 1, 1, 1), 0);
	Renderer::getSingleton()->registerVisible(canvas, 1);
	Renderer::getSingleton()->registerVisible(
			new TextField(GLUT_BITMAP_HELVETICA_18, (char *) "Current Zoom:\n",
					525, 400, makeColor4i(0, 0, 0, 255)), 1);
	Renderer::getSingleton()->registerVisible(
			new TextField(GLUT_BITMAP_HELVETICA_18, (str[21] = '\n', str),
					525, 480, makeColor4i(0, 0, 0, 255)), 1);
	Renderer::getSingleton()->registerVisible(
			new TextField(GLUT_BITMAP_HELVETICA_18,
					(char *) "Current Position:\n", 525, 200,
					makeColor4i(0, 0, 0, 255)), 1);
	Renderer::getSingleton()->registerVisible(click_pos, 1);

	TextInputField *t1 = new TextInputField(GLUT_BITMAP_HELVETICA_18, 525,
			390 - glutBitmapHeight(GLUT_BITMAP_HELVETICA_18), 100, 1,
			(unsigned char *) "0.5", text1, canvas);
	Renderer::getSingleton()->registerVisible(t1, 1);
	MouseReceiver::getSingleton()->registerObserver(t1);
	KeyboardReceiver::getSingleton()->registerObserver(t1);
	FocusMediator::getSingleton()->registerFocusable(t1);
	IdleMediator::getSingleton()->registerIdle(Renderer::getSingleton());

	TextInputField *t2 = new TextInputField(GLUT_BITMAP_HELVETICA_18, 525,
			190 - glutBitmapHeight(GLUT_BITMAP_HELVETICA_18), 100, 1,
			(unsigned char *) "0", text2, canvas);
	Renderer::getSingleton()->registerVisible(t2, 1);
	MouseReceiver::getSingleton()->registerObserver(t2);
	KeyboardReceiver::getSingleton()->registerObserver(t2);
	FocusMediator::getSingleton()->registerFocusable(t2);

	TextInputField *t3 = new TextInputField(GLUT_BITMAP_HELVETICA_18, 525,
			180 - 2 * glutBitmapHeight(GLUT_BITMAP_HELVETICA_18), 100, 1,
			(unsigned char *) "0", text3, canvas);
	Renderer::getSingleton()->registerVisible(t3, 1);
	MouseReceiver::getSingleton()->registerObserver(t3);
	KeyboardReceiver::getSingleton()->registerObserver(t3);
	FocusMediator::getSingleton()->registerFocusable(t3);

	Renderer::getSingleton()->registerVisible(
			new TextField(GLUT_BITMAP_HELVETICA_18,
					(char *) "Number of Cycles:\n", 525, 300,
					makeColor4i(0, 0, 0, 255)), 1);

	TextInputField *t4 = new TextInputField(GLUT_BITMAP_HELVETICA_18, 525,
			290 - glutBitmapHeight(GLUT_BITMAP_HELVETICA_18), 100, 1,
			(unsigned char *) "100", text4, canvas);
	Renderer::getSingleton()->registerVisible(t4, 1);
	MouseReceiver::getSingleton()->registerObserver(t4);
	KeyboardReceiver::getSingleton()->registerObserver(t4);
	FocusMediator::getSingleton()->registerFocusable(t4);

	MouseReceiver::getSingleton()->registerObserver(canvas);


	glutMainLoop();

	return (0);
}
