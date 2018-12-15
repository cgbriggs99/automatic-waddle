/*
 * Visible.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef VISIBLE_HPP_
#define VISIBLE_HPP_

#include "../namespace_name.hpp"
#include "base.hpp"

namespace NAME {

class Visible {
public:
	virtual ~Visible() {};

	virtual void draw() = 0;
};

}

#endif /* VISIBLE_HPP_ */
