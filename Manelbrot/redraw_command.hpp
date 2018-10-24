/*
 * redraw_command.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef REDRAW_COMMAND_HPP_
#define REDRAW_COMMAND_HPP_

#include "./command.hpp"
#include "./renderer.hpp"

class RedrawCommand : Command {
public:
	void execute() override {
		Renderer::getSingleton()->render();
	}
};

#endif /* REDRAW_COMMAND_HPP_ */
