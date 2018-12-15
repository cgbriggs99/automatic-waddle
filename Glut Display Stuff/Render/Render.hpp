/*
 * Render.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "../namespace_name.hpp"
#include "../Base/base.hpp"
#include "../Commands/commands.hpp"
#include "../../Design Patterns/design_patterns.hpp"

namespace NAME {

#include <vector>

class Layer {
private:
	std::vector<Visible *> contents;
public:
	void render();
};

class Renderer : public Observer, public design_patterns::Singleton<Renderer> {
private:
	std::vector<Layer> layers;
public:

	void receiveReport(Report &report);
};

}

#endif /* RENDER_HPP_ */
