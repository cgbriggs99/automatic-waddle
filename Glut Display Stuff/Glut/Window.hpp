/*
 * Window.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "../namespace_name.hpp"

namespace NAME {

class Window {
private:
	int width, height, x, y;
	bool visible;

public:
	Window(int x, int y, int width, int height, const unsigned char *name);
	~Window();

	int getWidth();
	int getHeight();
	int getX();
	int getY();

	void setWidth(int width);
	void setHeight(int height);
	void setX(int x);
	void setY(int y);

	bool isVisible();
	void setVisible(bool visible);

};

}

#endif /* WINDOW_HPP_ */
