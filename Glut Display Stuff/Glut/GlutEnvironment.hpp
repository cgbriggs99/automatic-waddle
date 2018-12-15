/*
 * Glut.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef GLUTENVIRONMENT_HPP_
#define GLUTENVIRONMENT_HPP_

#include "../namespace_name.hpp"
#include "glut.hpp"


#include "../../Design Patterns/design_patterns.hpp"

namespace NAME {

class GlutEnvironment : public design_patterns::Singleton<GlutEnvironment> {
private:
	Window *window;
	GlutEnvironment() {
		window = nullptr;
	}

	static void display();
	static void mouse(int button, int status, int x, int y);
	static void motion(int x, int y);
	static void passive(int x, int y);
	static void keydown(unsigned char key, int x, int y);
	static void keyup(unsigned char key, int x, int y);
	static void specialdown(int key, int x, int y);
	static void specialup(int key, int x, int y);
	static void idle();
	static void reshape(int wid, int hei);
	static void visibility(int status);
	static void entry(int status);
public:

	void initialize(int *argc, char **argv, int width, int height,
	                int winx, int winy, const unsigned char *name);

	const Window *getWindow();

};

}

#endif /* GLUTENVIRONMENT_HPP_ */
