/*
 * singleton.hpp
 *
 *  Created on: Oct 26, 2018
 *      Author: cgbri
 */

#ifndef CREATIONAL_SINGLETON_HPP_
#define CREATIONAL_SINGLETON_HPP_

namespace design_patterns {

template<typename _T>
class Singleton {
protected:
	static _T *singleton;

public:
	virtual ~Singleton() = 0;

	static _T *getSingleton() {
		if(singleton == nullptr) {
			singleton = new _T();
		}
		return (singleton);
	}
};
}

#endif /* CREATIONAL_SINGLETON_HPP_ */
