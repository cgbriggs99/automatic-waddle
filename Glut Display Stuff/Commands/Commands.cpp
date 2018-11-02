/*
 * Commands.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */


#include "../Commands/commands.hpp"
#include "../Commands/Display.hpp"

void NAME::DisplayCommand::execute() {
	DisplayReport report;
	NAME::ObservableConnector::getSingleton()->sendReport(report);
}


NAME::ReshapeCommand::ReshapeCommand(int wid, int hei) {
	this->height = hei;
	this->width = wid;
}

void NAME::ReshapeCommand::execute() {
	ReshapeReport report(this->width, this->height);
	NAME::ObservableConnector::getSingleton()->sendReport(report);
}

NAME::ReshapeReport::ReshapeReport(int wid, int hei) {
	this->height = hei;
	this->width = wid;
}

int NAME::ReshapeReport::getWidth() {
	return (this->width);
}

int NAME::ReshapeReport::getHeight() {
	return (this->height);
}

NAME::KeyboardCommand::KeyboardCommand(unsigned char key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

void NAME::KeyboardCommand::execute() {
	KeyboardReport report(key, x, y);
	NAME::ObservableConnector::getSingleton()->sendReport(report);
}

NAME::KeyboardReport::KeyboardReport(unsigned char key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

unsigned char NAME::KeyboardReport::getKey() {
	return (this->key);
}

int NAME::KeyboardReport::getX() {
	return (this->x);
}

int NAME::KeyboardReport::getY() {
	return (this->y);
}

NAME::SpecialCommand::SpecialCommand(int key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

void NAME::SpecialCommand::execute() {
	SpecialReport report(key, x, y);
	NAME::ObservableConnector::getSingleton()->sendReport(report);
}

NAME::SpecialReport::SpecialReport(int key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

int NAME::SpecialReport::getKey() {
	return (this->key);
}

int NAME::SpecialReport::getX() {
	return (this->x);
}

int NAME::SpecialReport::getY() {
	return (this->y);
}

NAME::KeyboardUpCommand::KeyboardUpCommand(unsigned char key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

void NAME::KeyboardUpCommand::execute() {
	KeyboardUpReport report(key, x, y);
	NAME::ObservableConnector::getSingleton()->sendReport(report);
}

NAME::KeyboardUpReport::KeyboardUpReport(unsigned char key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

unsigned char NAME::KeyboardUpReport::getKey() {
	return (this->key);
}

int NAME::KeyboardUpReport::getX() {
	return (this->x);
}

int NAME::KeyboardUpReport::getY() {
	return (this->y);
}

NAME::SpecialUpCommand::SpecialUpCommand(int key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

void NAME::SpecialUpCommand::execute() {
	SpecialUpReport report(key, x, y);
	NAME::ObservableConnector::getSingleton()->sendReport(report);
}

NAME::SpecialUpReport::SpecialUpReport(int key, int x, int y) {
	this->key = key;
	this->x = x;
	this->y = y;
}

int NAME::SpecialUpReport::getKey() {
	return (this->key);
}

int NAME::SpecialUpReport::getX() {
	return (this->x);
}

int NAME::SpecialUpReport::getY() {
	return (this->y);
}
