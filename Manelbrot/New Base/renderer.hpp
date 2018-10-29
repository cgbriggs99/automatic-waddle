/*
 * renderer.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: cgbri
 */

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <vector>
#include "visible.hpp"
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "Design Patterns/design_patterns.hpp"
#include "command.hpp"
#include "glutcommands.hpp"

class Layer {
private:
	std::vector<Visible *> vis;
public:
	Layer() {
		;
	}

	void registerVisible(Visible *vis) {
		this->vis.push_back(vis);
	}

	void render() {
		for(int i = 0; i < vis.size(); i++) {
			vis[i]->draw();
		}
	}
};

class Renderer : public design_patterns::Singleton<Renderer>, public Receiver {
private:
	std::vector<Layer *> layers;

public:
	void registerVisible(Visible *vis, int layer) {
		if(layers.size() <= layer) {
			for(int i = layers.size(); i <= layer; i++) {
				layers.push_back(new Layer());
			}
		} else {
			layers[layer]->registerVisible(vis);
		}
	}

	void receive(GlutDisplayCommand &command) override {
		for(int i = 0; i < layers.size(); i++) {
			layers[i]->render();
		}
	}

	void receive(GlutIdleCommand &command) override {
		for(int i = 0; i < layers.size(); i++) {
			layers[i]->render();
		}
	}
};

#endif /* RENDERER_HPP_ */
