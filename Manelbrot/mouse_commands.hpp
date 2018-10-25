/*
 * mouse_commands.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef MOUSE_COMMANDS_HPP_
#define MOUSE_COMMANDS_HPP_

#include <vector>

class MouseObserver {
public:
	MouseObserver() {
		;
	}

	virtual ~MouseObserver() {}

	virtual void onClick(int button, int status, int x, int y) = 0;

};

class MouseReceiver {
private:
	static MouseReceiver *singleton;

	std::vector<MouseObserver *> observers;

	MouseReceiver() {
		;
	}
public:
	static MouseReceiver *getSingleton() {
		if(singleton == nullptr) {
			singleton = new MouseReceiver();
		}
		return (singleton);
	}

	void registerObserver(MouseObserver *obs) {
		this->observers.push_back(obs);
	}

	void update(int button, int status, int x, int y) {
		for(int i = 0; i < observers.size(); i++) {
			observers[i]->onClick(button, status, x, y);
		}
	}
};

#endif /* MOUSE_COMMANDS_HPP_ */
