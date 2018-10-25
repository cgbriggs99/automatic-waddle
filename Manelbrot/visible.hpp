/*
 * visible.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef VISIBLE_HPP_
#define VISIBLE_HPP_

#include "./defs.hpp"
#include "./command.hpp"

class Visible {
public:
	virtual ~Visible() {
		;
	}
	virtual void draw() = 0;
};

#endif /* VISIBLE_HPP_ */
