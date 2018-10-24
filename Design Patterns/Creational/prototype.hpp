/*
 * prototype.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: cgbri
 */

#ifndef CREATIONAL_PROTOTYPE_HPP_
#define CREATIONAL_PROTOTYPE_HPP_

namespace design_patterns {

class Prototype {
public:
	virtual Prototype();
	virtual ~Prototype();

	virtual Prototype clone();
};

}

#endif /* CREATIONAL_PROTOTYPE_HPP_ */
