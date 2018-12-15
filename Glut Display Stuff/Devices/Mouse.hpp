/*
 * Mouse.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include "../namespace_name.hpp"

namespace NAME {

class Mouse {
private:
	static Mouse *singleton;

	uint32_t buttons;
	int last_x, last_y;
	bool is_in;

	Mouse() {
		this->buttons = 0;
		this->last_x = -1;
		this->last_y = -1;
		this->is_in = true;
	}
public:
	static Mouse *getSingleton();

	bool isButtonPressed(int button);
	void setButtonPressed(int button, bool pressed);

	int getLastX();
	int getLastY();
	void setLastX(int x);
	void setLastY(int y);

	bool isInWindow();
	void setInWindow(bool in);
};

}

#endif /* MOUSE_HPP_ */
