/*
 * Visibility.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef VISIBILITY_HPP_
#define VISIBILITY_HPP_

#include "../namespace_name.hpp"
#include "commands.hpp"

namespace NAME {

class VisibilityCommand : public Command {
public:
	VisibilityCommand(int status);
	void execute();
private:
	int status;
};

class VisibilityReport : public Report {
public:
	VisibilityReport(int status);
	int getStatus();
private:
	int status;
};

}

#endif /* VISIBILITY_HPP_ */
