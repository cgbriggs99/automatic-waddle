/*
 * observer.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: connor
 */

#ifndef BEHAVIORAL_OBSERVER_HPP_
#define BEHAVIORAL_OBSERVER_HPP_

#include <vector>

namespace design_patterns {

class Observer {
	virtual ~Observer() = 0;

	template<typename ...Args>
	virtual void update(Args...) = 0;
};

class Subject {
protected:
	std::vector<Observer *> observers;
public:
	virtual ~Subject() = 0;

	virtual void registerOberserver(Observer *obs) {
		this->observers.push_back(obs);
	}

	virtual void update(Args... args) {
		for(int i = 0; i < this->observers.size(); i++) {
			this->observers[i]->update(args);
		}
	}
};

}

#endif /* BEHAVIORAL_OBSERVER_HPP_ */
