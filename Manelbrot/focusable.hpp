/*
 * focusable.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef FOCUSABLE_HPP_
#define FOCUSABLE_HPP_

#include <vector>

class Focusable {
protected:
	bool hasFocus;
public:
	Focusable() {
		hasFocus = false;
	}

	virtual ~Focusable(){}

	bool isFocused() {
		return (hasFocus);
	}

	void setFocus(bool focus) {
		hasFocus = focus;
	}
};

class FocusMediator {
private:
	std::vector<Focusable *> refs;
	Focusable *curr_focused;

	static FocusMediator *singleton;
	FocusMediator() {
		curr_focused = nullptr;
	}
public:
	~FocusMediator() {
		;
	}

	static FocusMediator *getSingleton() {
		if(singleton == nullptr) {
			singleton = new FocusMediator();
		}
		return (singleton);
	}

	void giveFocus(Focusable *f) {
		for(int i = 0; i < refs.size(); i++) {
			if(refs[i] == f) {
				f->setFocus(true);
				curr_focused = f;
			} else {
				refs[i]->setFocus(false);
			}
		}
	}

	void removeFocus(Focusable *f) {
		if(f->isFocused()) {
			curr_focused = nullptr;
			f->setFocus(false);
		}
	}

	void registerFocusable(Focusable *f) {
		refs.push_back(f);
	}
};

#endif /* FOCUSABLE_HPP_ */
