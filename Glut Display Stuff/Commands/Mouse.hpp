/*
 * Mouse.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include "../namespace_name.hpp"
#include "commands.hpp"

namespace NAME {

class MouseCommand : public Command {
public:
	MouseCommand(int button, int status, int x, int y);

	void execute();
private:
	int button, status, x, y;
};

class MouseReport : public Report {
public:
	MouseReport(int button, int status, int x, int y);

	int getButton();
	int getStatus();
	int getX();
	int getY();
private:
	int button, status, x, y;
};

class MotionCommand : public Command {
public:
	MotionCommand(int x, int y);
	void execute();
private:
	int x, y;
};

class MotionReport : public Report {
public:
	MotionReport(int x, int y);

	int getX();
	int getY();
private:
	int x, y;
};

class PassiveMotionCommand : public Command {
public:
	PassiveMotionCommand(int x, int y);
	void execute();
private:
	int x, y;
};

class PassiveMotionReport : public Report {
public:
	PassiveMotionReport(int x, int y);
	int getX();
	int getY();
private:
	int x, y;
};

class EntryCommand : public Command {
public:
	EntryCommand(int state);
	void execute();
private:
	int state;
};

class EntryReport : public Report {
public:
	EntryReport(int state);
	int getState();
private:
	int state;
};

}

#endif /* MOUSE_HPP_ */
