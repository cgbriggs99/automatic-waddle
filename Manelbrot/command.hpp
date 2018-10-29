/*
 * command.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <vector>

class Command {
public:
	virtual ~Command();

	virtual void execute();
};

class Observer {
public:
	virtual ~Observer() = 0;

	virtual void update() = 0;
};

class Receiver {
protected:
	std::vector<Observer *> observers;

public:
	virtual ~Receiver() {
		this->observers.clear();
	}

	virtual void registerObserver(Observer *obs) {
		this->observers.push_back(obs);
	}

	virtual void updateObservers() {
		for(int i = 0; i < this->observers.size(); i++) {
			this->observers[i]->update();
		}
	}

};

#endif /* COMMAND_HPP_ */
