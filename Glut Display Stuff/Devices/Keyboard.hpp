/*
 * keyboard.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include "../namespace_name.hpp"
#include <stdint.h>
#include <GL/freeglut.h>

namespace NAME {

class Keyboard {
private:
	static Keyboard *singleton;

	uint64_t keys1;
	uint64_t keys2;
	uint64_t specials;
	Keyboard() {
		this->keys1 = 0;
		this->keys2 = 0;
		this->specials = 0;
	}
public:
	static Keyboard *getSingleton();

	bool isKeyPressed(unsigned char key);
	bool isSpecialPressed(int key);

	void setKeyPressed(unsigned char key, bool pressed);
	void setSpecialPressed(int key, bool pressed);
};

}


#endif /* KEYBOARD_HPP_ */
