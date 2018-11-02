/*
 * Command.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "../namespace_name.hpp"

namespace NAME {

class Command {
public:
	virtual ~Command() {};
	virtual bool execute() = 0;
};

}

#endif /* COMMAND_HPP_ */
