/*
 * Observer.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include "../namespace_name.hpp"
#include "./base.hpp"

namespace NAME {

class Observer {
public:
	virtual ~Observer() {};

	virtual void receiveReport(Report &r);
};

}
#endif /* OBSERVER_HPP_ */
