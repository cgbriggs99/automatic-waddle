/*
 * keyboard_commands.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef KEYBOARD_COMMANDS_HPP_
#define KEYBOARD_COMMANDS_HPP_

#include <vector>

class KeyboardObserver {
public:
	KeyboardObserver() {
		;
	}

	virtual ~KeyboardObserver() {}

	virtual void onKeyPress(unsigned char key, int x, int y) = 0;

};

class KeyboardReceiver {
private:
	static KeyboardReceiver *singleton;

	std::vector<KeyboardObserver *> observers;

	KeyboardReceiver() {
		;
	}
public:
	static KeyboardReceiver *getSingleton() {
		if(singleton == nullptr) {
			singleton = new KeyboardReceiver();
		}
		return (singleton);
	}

	void registerObserver(KeyboardObserver *obs) {
		this->observers.push_back(obs);
	}

	void update(unsigned char key, int x, int y) {
		for(int i = 0; i < observers.size(); i++) {
			observers[i]->onKeyPress(key, x, y);
		}
	}
};

#endif /* KEYBOARD_COMMANDS_HPP_ */
