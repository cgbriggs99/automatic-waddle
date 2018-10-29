/*
 * hid.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: cgbri
 */

#ifndef HID_HPP_
#define HID_HPP_

#include "Design Patterns/design_patterns.hpp"
#include <stdint.h>
#include "command.hpp"
#include "glutcommands.hpp"

class Keyboard : public design_patterns::Singleton<Keyboard>, public Receiver {
private:
	uint64_t key_states;
	uint32_t command_states;
	uint32_t special_states;
public:
	bool getKeyState(unsigned char key) {
		return ((key_states & (1 << (key - 32))) != 0);
	}

	bool getCommandState(unsigned char key) {
		return ((command_states & (1 << key)) != 0);
	}

	bool getSpecialState(int key) {
		if(key < 0x64) {
			return ((special_states & (1 << key)) != 0);
		} else {
			return ((special_states & (1 << (key - 0x57))));
		}
	}

	static unsigned char remapModifiedKey(unsigned char key) {
		unsigned char remapped;
		if(key >= 'a' && key <= 'z') {
			remapped = key - 32;
		}
		switch(key) {
		//Since tilde and grave are high up, turn them in to a different symbol.
		case '~':
		case '`':
			remapped = '!';
			break;
		case '!':
			remapped = '1';
			break;
		case '@':
			remapped = '2';
			break;
		case '#':
			remapped = '3';
			break;
		case '$':
			remapped = '4';
			break;
		case '%':
			remapped = '5';
			break;
		case '^':
			remapped = '6';
			break;
		case '&':
			remapped = '7';
			break;
		case '*':
			remapped = '8';
			break;
		case '(':
			remapped = '9';
			break;
		case ')':
			remapped = '0';
			break;
		case '_':
			remapped = '-';
			break;
		case '+':
			remapped = '=';
			break;
		case '{':
			remapped = '[';
			break;
		case '}':
			remapped = ']';

		}
	}

	bool setKeyState(unsigned char key) {

	}
};

class Mouse : public design_patterns::Singleton<Mouse>, public Receiver {
private:
	uint8_t buttons;
	int x, y;
	bool in_window;

	Mouse() {
		this->buttons = 0;
	}

public:

	void setX(int x) {
		this->x = x;
	}

	void setY(int y) {
		this->y = y;
	}

	void setButtonState(int button, bool state) {
		uint8_t mask = 1 << button;

		if(state) {
			this->buttons |= mask;
		} else {
			this->buttons &= ~mask;
		}
	}

	void setWindowState(bool state) {
		this->in_window = state;
	}

	int getX() {
		return (this->x);
	}

	int getY() {
		return (this->y);
	}

	bool isButtonPressed(int button) {
		return ((this->buttons & (1 << button)) != 0);
	}

	void receive(GlutMouseCommand &command) {
		this->setButtonState(command.getButton(), command.getStatus() == 1);
	}

	void receive(GlutMotionCommand &command) {
		this->setX(command.getX());
		this->setY(command.getY());
	}

};

#endif /* HID_HPP_ */
