/*
 * singleton.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: cgbri
 */

#ifndef CREATIONAL_SINGLETON_HPP_
#define CREATIONAL_SINGLETON_HPP_

namespace design_patterns {

class Singleton {
protected:
	static Singleton *singleton;

public :
	virtual ~Singleton();

	static virtual Singleton *getSingleton();
};

}

#endif /* CREATIONAL_SINGLETON_HPP_ */
