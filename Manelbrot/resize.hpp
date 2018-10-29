/*
 * resize.hpp
 *
 *  Created on: Oct 26, 2018
 *      Author: cgbri
 */

#ifndef RESIZE_HPP_
#define RESIZE_HPP_

class ResizeObserver {
public:
	virtual ~ResizeObserver() = 0;

	virtual void onResize(int wid, int hei) = 0;
};

class ResizeReceiver {
private:

};

#endif /* RESIZE_HPP_ */
