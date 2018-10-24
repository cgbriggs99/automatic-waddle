/*
 * command.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

class Command {
public:
	virtual ~Command();

	virtual void execute();
};

#endif /* COMMAND_HPP_ */
