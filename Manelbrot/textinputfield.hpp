/*
 * textinputfield.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef TEXTINPUTFIELD_HPP_
#define TEXTINPUTFIELD_HPP_

#include "focusable.hpp"
#include "mouse_commands.hpp"
#include "keyboard_commands.hpp"
#include "visible.hpp"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "window.hpp"

char shiftCharacter(char ch) {
	switch(ch) {
	case '1':
		return ('!');
	case '2':
		return ('@');
	case '3':
		return ('#');
	case '4':
		return ('$');
	case '5':
		return ('%');
	case '6':
		return ('^');
	case '7':
		return ('&');
	case '8':
		return ('*');
	case '9':
		return ('(');
	case '0':
		return (')');
	case '-':
		return ('_');
	case '=':
		return ('+');
	case '[':
		return ('{');
	case ']':
		return ('}');
	case '\\':
		return ('|');
	case ';':
		return (':');
	case '\'':
		return ('"');
	case ',':
		return ('<');
	case '.':
		return ('>');
	case '/':
		return ('?');
	case '`':
		return ('~');
	case ' ':
	case '\n':
	case '\b':
	case 127:
		return (ch);
	default:
		return (ch + 32);
	}
}

class TextInputField : public Visible, public Focusable, public MouseObserver, public KeyboardObserver {
private:
	unsigned char *str;
	int x, y;
	int width, height;
	int cursorpos;
	void *font;
	void *data;
	void (*onEntered)(const unsigned char *str, void *);
public:
	TextInputField(void *font, int x, int y, int width, int height, unsigned char *label,
			void (*onEntered)(const unsigned char*, void *ptr), void *data) {
		this->x = x;
		this->y = y;
		this->height = height * (glutBitmapHeight(font) + 6);
		this->str = (unsigned char *) calloc(strlen((char *) label) + 1, sizeof(char));
		memcpy(this->str, label, strlen((char *)label));
		this->width = width;
		this->cursorpos = 0;
		this->font = font;
		this->onEntered = onEntered;
		this->data = data;
	}

	virtual ~TextInputField() {
		free(this->str);
	}

	void draw() override {
		Window *w = Window::getSingleton();
		glBegin(GL_LINE_STRIP);
		glColor4f(0, 0, 0, 1);

		glVertex2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glVertex2f((2.0f * (x + width)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glVertex2f((2.0f * (x + width)) / w->getWidth() - 1, (2.0f * (y + height)) / w->getHeight() - 1);
		glVertex2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * (y + height)) / w->getHeight() - 1);
		glVertex2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glEnd();
		glRasterPos2f((2.0f * (x + 3)) / w->getWidth() - 1, (2.0f * (y + 3)) / w->getHeight() - 1);
		unsigned char *temp = (unsigned char *) calloc(strlen((char *) this->str) + 1, sizeof(char));
		strncpy((char *) temp, (char *) this->str, strlen((char *) this->str));
		while(glutBitmapLength(this->font, temp) > width - 6) {
			temp[strlen((char *) temp) - 1] = 0;
		}
		glutBitmapString(this->font, temp);
		free(temp);
	}

	void onClick(int button, int status, int x, int y) override {
		int val1 = Window::getSingleton()->getHeight() - y, val2 = this->y + this->height;
		if(x > this->x && x < this->x + this->width && (Window::getSingleton()->getHeight() - y) > this->y &&
				(Window::getSingleton()->getHeight() - y) < this->y + this->height) {
			FocusMediator::getSingleton()->giveFocus(this);
			if(hasFocus) {
				printf("I have focus.\n");
				cursorpos = strlen((char *) this->str);
			}
		} else {
			FocusMediator::getSingleton()->removeFocus(this);
			if(!hasFocus) {
				printf("I no longer have focus.\n");
			}
		}
	}

	void onKeyPress(unsigned char key, int x, int y) override {
		if(hasFocus) {
			printf("Recieved: %c (%d)\n", key, key);
			if(key == '\n' || key == '\r') {
				this->onEntered(this->str, this->data);
			} else if(key == '\b') {
				if(cursorpos > 0) {
					for(int i = cursorpos - 1; i < strlen((char *) str); i++) {
						str[i] = str[i + 1];
					}
					cursorpos--;
				}
			} else if(key == 127) {
				if(cursorpos < strlen((char *) str)) {
					for(int i = cursorpos; i < strlen((char *) str); i++) {
						str[i] = str[i + 1];
					}
				}
			} else {
				char ch = key;
				this->str = (unsigned char *) realloc(this->str, (strlen((char *) this->str) + 2) * sizeof(char));
				for(int i = strlen((char *) this->str); i > this->cursorpos; i--) {
					this->str[i] = this->str[i - 1];
				}
				this->str[cursorpos] = ch;
				cursorpos++;
			}
		}
	}
};

#endif /* TEXTINPUTFIELD_HPP_ */
