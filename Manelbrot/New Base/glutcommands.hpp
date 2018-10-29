/*
 * glutcommands.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: cgbri
 */

#ifndef GLUTCOMMANDS_HPP_
#define GLUTCOMMANDS_HPP_

#include "command.hpp"

class GlutDisplayCommand : public Command {
};

class GlutReshapeCommand : public Command {
private:
	int width, height;
public:
	GlutReshapeCommand(int width, int height) {
		this->width = width;
		this->height = height;
	}

	int getWidth() {
		return (this->width);
	}

	int getHeight() {
		return (this->height);
	}
};

class GlutKeyboardCommand : public Command {
private:
	int x, y;
	unsigned char key;
public:
	GlutKeyboardCommand(unsigned char key, int x, int y) {
		this->x = x;
		this->y = y;
		this->key = key;
	}

	unsigned char getKey() {
		return (this->key);
	}

	int getX() {
		return (this->x);
	}

	int getY() {
		return (this->y);
	}
};

class GlutMouseCommand : public Command {
private:
	int button, status, x, y;
public:
	GlutMouseCommand(int button, int status, int x, int y) {
		this->button = button;
		this->status = status;
		this->x = x;
		this->y = y;
	}

	int getButton() {
		return (this->button);
	}

	int getStatus() {
		return (this->status);
	}

	int getX() {
		return (this->x);
	}

	int getY() {
		return (this->y);
	}
};

class GlutPassiveCommand : public Command {
private:
	int x, y;
public:
	GlutPassiveCommand(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int getX() {
		return (x);
	}

	int getY() {
		return (y);
	}
};

class GlutMotionCommand : public Command {
private:
	int x, y;
public:
	GlutMotionCommand(int x ,int y) {
		this->x = x;
		this->y = y;
	}

	int getX() {
		return (this->x);
	}

	int getY() {
		return (this->y);
	}
};

class GlutVisibilityCommand : public Command {
private:
	int state;
public:
	GlutVisibilityCommand(int state) {
		this->state = state;
	}

	int getState() {
		return (this->state);
	}
};

class GlutEntryCommand : public Command {
private:
	int state;
public:
	GlutEntryCommand(int state) {
		this->state = state;
	}

	int getState() {
		return (this->state);
	}
};

class GlutSpecialCommand : public Command {
private:
	int key, x, y;
public:
	GlutSpecialCommand(int key, int x, int y) {
		this->key = key;
		this->x = x;
		this->y = y;
	}

	int getX() {
		return (this->x);
	}

	int getY() {
		return (this->y);
	}

	int getKey() {
		return (this->key);
	}
};

class GlutIdleCommand : public Command {

};

#endif /* GLUTCOMMANDS_HPP_ */
