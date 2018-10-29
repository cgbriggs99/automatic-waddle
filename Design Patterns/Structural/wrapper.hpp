/*
 * wrapper.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: cgbri
 */

#ifndef STRUCTURAL_WRAPPER_HPP_
#define STRUCTURAL_WRAPPER_HPP_

namespace design_patterns {

class Wrapper {
public:
	virtual ~Wrapper() = 0;

	virtual void wrapperMethod() = 0;
};

}

#endif /* STRUCTURAL_WRAPPER_HPP_ */
