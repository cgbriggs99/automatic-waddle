/*
 * Glut.cpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */


#include "glut.hpp"
#include <GL/glut.h>

void NAME::GlutEnvironment::display() {
	NAME::ObservableConnector::getSingleton();
}

