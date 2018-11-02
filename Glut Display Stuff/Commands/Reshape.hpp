/*
 * Reshape.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef RESHAPE_HPP_
#define RESHAPE_HPP_

#include "../namespace_name.hpp"
#include "commands.hpp"

namespace NAME {

class ReshapeCommand : public Command {
public:
	ReshapeCommand(int wid, int hei);

	void execute();
private:
	int width, height;
};

class ReshapeReport : public Report {
public:
	ReshapeReport(int wid, int hei);

	int getWidth();
	int getHeight();
private:
	int width, height;
};

}

#endif /* RESHAPE_HPP_ */
