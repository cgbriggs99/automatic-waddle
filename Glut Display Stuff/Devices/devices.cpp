/*
 * devices.cpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#include "devices.hpp"
#include <GL/glut.h>

static unsigned char keyhash(unsigned char key) {
	unsigned char key_pressed = key;
	char mods = glutGetModifiers();

	if(mods & GLUT_ACTIVE_SHIFT) {
		switch(key) {
		case '~':
			key_pressed = '`';
			break;
		case '!':
			key_pressed = '1';
			break;
		case '@':
			key_pressed = '2';
			break;
		case '#':
			key_pressed = '3';
			break;
		case '$':
			key_pressed = '4';
			break;
		case '%':
			key_pressed = '5';
			break;
		case '^':
			key_pressed = '6';
			break;
		case '&':
			key_pressed = '7';
			break;
		case '*':
			key_pressed = '8';
			break;
		case '(':
			key_pressed = '9';
			break;
		case ')':
			key_pressed = '0';
			break;
		case '_':
			key_pressed = '-';
			break;
		case '+':
			key_pressed = '=';
			break;
		case '{':
			key_pressed = '[';
			break;
		case '}':
			key_pressed = ']';
			break;
		case '|':
			key_pressed = '\\';
			break;
		case ':':
			key_pressed = ';';
			break;
		case '"':
			key_pressed = '\'';
			break;
		case '<':
			key_pressed = ',';
			break;
		case '>':
			key_pressed = '.';
			break;
		case '?':
			key_pressed = '/';
			break;
		default:
			if(key >= 'A' && key <= 'Z') {
				key_pressed = key + 0x20;
			}
		}
	}
	return (key_pressed);
}

NAME::Keyboard *NAME::Keyboard::getSingleton() {
	if(NAME::Keyboard::singleton == nullptr) {
		NAME::Keyboard::singleton = new Keyboard();
	}
	return (NAME::Keyboard::singleton);
}

bool NAME::Keyboard::isKeyPressed(unsigned char key) {
	unsigned char key_code = keyhash(key);
	if(key_code < 64) {
		return ((this->keys1 & (1 << key_code)) != 0);
	} else {
		return ((this->keys2 & (1 << (key_code - 64))) != 0);
	}
}

bool NAME::Keyboard::isSpecialPressed(int key) {
	if(key >= 0x64) {
		return ((this->specials & (1 << (key - 0x5a))) != 0);
	} else {
		return ((this->specials & (1 << key)) != 0);
	}
}

void NAME::Keyboard::setKeyPressed(unsigned char key, bool pressed) {
	unsigned char key_code = keyhash(key);
	if(key_code < 64) {
		if(pressed) {
			this->keys1 |= 1 << key_code;
		} else {
			this->keys1 &= ~(1 << key_code);
		}
	} else {
		if(pressed) {
			this->keys2 |= 1 << (key_code - 64);
		} else {
			this->keys2 &= ~(1 << (key_code - 64));
		}
	}
}

void NAME::Keyboard::setSpecialPressed(int key, bool pressed) {
	if(key >= 0x64) {
		if(pressed) {
			this->specials |= 1 << (key - 0x5a);
		} else {
			this->specials &= ~(1 << (key - 0x5a));
		}
	} else {
		if(pressed) {
			this->specials |= 1 << key;
		} else {
			this->specials &= ~(1 << key);
		}
	}
}

int NAME::Mouse::getLastX() {
	return (this->last_x);
}

int NAME::Mouse::getLastY() {
	return (this->last_y);
}

NAME::Mouse *NAME::Mouse::getSingleton() {
	if(NAME::Mouse::singleton == nullptr) {
		NAME::Mouse::singleton = new NAME::Mouse();
	}
	return (NAME::Mouse::singleton);
}

bool NAME::Mouse::isButtonPressed(int button) {
	return ((this->buttons & (1 << button)) != 0);
}

bool NAME::Mouse::isInWindow() {
	return (this->is_in);
}

void NAME::Mouse::setButtonPressed(int button, bool status) {
	if(status) {
		this->buttons |= 1 << button;
	} else {
		this->buttons &= ~(1 << button);
	}
}

void NAME::Mouse::setInWindow(bool in) {
	this->is_in = in;
}

void NAME::Mouse::setLastX(int x) {
	this->last_x = x;
}

void NAME::Mouse::setLastY(int y) {
	this->last_y = y;
}

