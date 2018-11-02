/*
 * Idle.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef IDLE_HPP_
#define IDLE_HPP_

#include "../namespace_name.hpp"
#include "commands.hpp"

namespace NAME {

class IdleCommand : public Command {
public:
	IdleCommand();
	void execute();
};

class IdleReport : public Report {
public:
	IdleReport();
};

}

#endif /* IDLE_HPP_ */
