/*
 * Display.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include "../Commands/commands.hpp"
#include "../namespace_name.hpp"

namespace NAME {

class DisplayCommand : public Command {
public :
	void execute();
};

class DisplayReport : public Report {
public :

};

}

#endif /* DISPLAY_HPP_ */
