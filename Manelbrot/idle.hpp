/*
 * idle.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef IDLE_HPP_
#define IDLE_HPP_

#include <vector>

class Idle {
public:
	virtual ~Idle(){}

	virtual void onIdle(void) = 0;
	virtual int getWaitMilis() = 0;
	virtual bool getDoIdle() = 0;
};

class IdleMediator {
private:
	std::vector<Idle *> obs;

	static IdleMediator *singleton;
	IdleMediator() {
		;
	}

	static void idle_func(int status) {
		IdleMediator::getSingleton()->obs[status]->onIdle();
	}
public:
	static IdleMediator *getSingleton() {
		if(singleton == nullptr) {
			singleton = new IdleMediator();
		}
		return (singleton);
	}

	void registerIdle(Idle *idle) {
		obs.push_back(idle);
	}

	void loopIdle() {
		for(int i = 0; i < obs.size(); i++) {
			if(obs[i]->getDoIdle()) {
				glutTimerFunc(obs[i]->getWaitMilis(), IdleMediator::idle_func, i);
			}
		}
	}
};

#endif /* IDLE_HPP_ */
