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
#include "update_command.hpp"
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <pthread.h>
#include "idle.hpp"
//#include <C:/freeglut/include/GL/freeglut.h>

class Layer {
private:
	Visible **to_draw;
	int len;

	static void *drawfunc(void *v) {
		((Visible *) v)->draw();
		glutSwapBuffers();
		return (NULL);
	}
public:
	Layer() {
		this->to_draw = (Visible **) calloc(0, sizeof(Visible *));
		this->len = 0;
	}

	~Layer() {
		for(int i = 0; i < len; i++) {
			delete(this->to_draw[i]);
		}
		free(this->to_draw);
	}

	void registerVisible(Visible *v) {
		this->to_draw = (Visible **) realloc(this->to_draw, (1 + this->len) * sizeof(Visible *));
		this->to_draw[this->len] = v;
		this->len++;
	}

	void render() {
		for(int i = 0; i < len; i++) {
			this->to_draw[i]->draw();
		}
	}

};

class Renderer : public UpdateCommandObserver, public Idle {
private:
	Layer **layers;
	int len;
	int width, height;

	color_t *data;
	static Renderer *singleton;

	Renderer() {
		layers = (Layer **) calloc(0, sizeof(Visible *));
		this->width = glutGet(GLUT_WINDOW_WIDTH);
		this->height = glutGet(GLUT_WINDOW_HEIGHT);
		this->len = 0;
		this->data = (color_t *) calloc(this->width * this->height, sizeof(color_t));
		needs_update = true;
		UpdateCommandReceiver::getSingleton()->registerObserver(this);
	}

public :
	~Renderer() {
		for(int i = 0; i < len; i++) {
			delete(layers[i]);
		}
		free(layers);
	}

	static Renderer *getSingleton() {
		if(Renderer::singleton == nullptr) {
			Renderer::singleton = new Renderer();
		}
		return (singleton);
	}

	void registerVisible(Visible *v, int layer) {
		if(layer < len) {
			this->layers[layer]->registerVisible(v);
		} else {
			this->layers = (Layer **) realloc(this->layers, (layer + 1) * sizeof(Layer));
			for(int i = len; i <= layer; i++) {
				this->layers[i] = new Layer();
			}
			this->len = layer + 1;
			this->layers[layer]->registerVisible(v);
		}
	}

	void changeWindowSize(int new_width, int new_height) {
		this->data = (color_t *) realloc(this->data, new_width * new_height * sizeof(color_t));
	}

	void render() {
		if(needs_update) {
			for(int i = 0; i < len; i++) {
				this->layers[i]->render();
				glutSwapBuffers();
			}
			glutSwapBuffers();
			needs_update = false;
		}
	}

	int getWaitMilis() override {
		return (1000);
	}

	void onIdle() {
		render();
	}
};



#endif /* RENDERER_HPP_ */
