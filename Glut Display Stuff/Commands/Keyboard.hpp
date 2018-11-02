/*
 * Keyboard.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include "../namespace_name.hpp"
#include "commands.hpp"

namespace NAME {

class KeyboardCommand : public Command {
public:
	KeyboardCommand(unsigned char key, int x, int y);

	void execute();
private:
	unsigned char key;
	int x, y;
};

class SpecialCommand : public Command {
public:
	SpecialCommand(int key, int x, int y);

	void execute();
private:
	int key, x, y;
};

class KeyboardReport : public Report {
public:
	KeyboardReport(unsigned char key, int x, int y);

	int getX();
	int getY();
	unsigned char getKey();
private:
	unsigned char key;
	int x, y;
};

class SpecialReport : public Report {
public:
	SpecialReport(int key, int x, int y);

	int getKey();
	int getX();
	int getY();
private:
	int key, x, y;
};

class KeyboardUpCommand : public Command {
public:
	KeyboardUpCommand(unsigned char key, int x, int y);

	void execute();
private:
	unsigned char key;
	int x, y;
};

class SpecialUpCommand : public Command {
public:
	SpecialUpCommand(int key, int x, int y);

	void execute();
private:
	int key, x, y;
};

class KeyboardUpReport : public Report {
public:
	KeyboardUpReport(unsigned char key, int x, int y);

	int getX();
	int getY();
	unsigned char getKey();
private:
	unsigned char key;
	int x, y;
};

class SpecialUpReport : public Report {
public:
	SpecialUpReport(int key, int x, int y);

	int getKey();
	int getX();
	int getY();
private:
	int key, x, y;
};

}

#endif /* KEYBOARD_HPP_ */
