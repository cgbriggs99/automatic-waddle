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
	unsigned char *label;
	void (*onEntered)(const unsigned char *str, void *);
	bool show_label;
public:
	TextInputField(void *font, int x, int y, int width, int height, unsigned char *label,
			void (*onEntered)(const unsigned char*, void *ptr), void *data) {
		this->x = x;
		this->y = y;
		this->height = height * (glutBitmapHeight(font) + 6);
		this->label = (unsigned char *) calloc(strlen((char *) label) + 1, sizeof(char));
		memcpy(this->label, label, strlen((char *)label));
		this->width = width;
		this->cursorpos = 0;
		this->font = font;
		this->onEntered = onEntered;
		this->data = data;
		this->str = (unsigned char *) calloc(1, sizeof(char));
		this->show_label = true;
	}

	virtual ~TextInputField() {
		if(this->str != NULL) {
			free(this->str);
		}
		if(this->label != NULL) {
			free(this->label);
		}
	}

	void draw() override {
	Window *w = Window::getSingleton();
		glBegin(GL_LINE_STRIP);

		if(strlen((char *) this->str) == 0) {
			glColor4f(0, 0, 0, 0.7);
		} else {
			glColor4f(0, 0, 0, 1);
		}

		glVertex2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glVertex2f((2.0f * (x + width)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glVertex2f((2.0f * (x + width)) / w->getWidth() - 1, (2.0f * (y + height)) / w->getHeight() - 1);
		glVertex2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * (y + height)) / w->getHeight() - 1);
		glVertex2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
		glEnd();
		glRasterPos2f((2.0f * (x + 3)) / w->getWidth() - 1, (2.0f * (y + 3)) / w->getHeight() - 1);
		unsigned char *temp;
		if(strlen((char *) this->str) == 0 && strlen((char *) this->label) != 0 &&
				this->show_label) {
			temp = (unsigned char *) calloc(strlen((char *) this->label) + 1, sizeof(char));
			strncpy((char *) temp, (char *) this->label, strlen((char *) this->label));
		} else if(strlen((char *) this->str) != 0){
			temp = (unsigned char *) calloc(strlen((char *) this->str) + 1, sizeof(char));
			strncpy((char *) temp, (char *) this->str, strlen((char *) this->str));
		} else {
			temp = (unsigned char *) calloc(1, sizeof(char));
		}
		while(glutBitmapLength(this->font, temp) > width - 6) {
			temp[strlen((char *) temp) - 1] = 0;
		}

		for(int i = 0; i < strlen((char *) temp); i++) {
			glutBitmapCharacter(this->font, temp[i]);
			if(this->cursorpos == i && this->hasFocus) {
				drawCursor();
			}
		}
		if(this->cursorpos == strlen((char *) temp) && this->hasFocus) {
			drawCursor();
		}

		free(temp);
	}

	void drawCursor() {
		color_t *cursor = (color_t *) calloc(glutBitmapHeight(this->font),
			sizeof(color_t));
		for(int i = 0; i < glutBitmapHeight(this->font); i++) {
			cursor[i] = makeColor4i(0, 0, 0, 255);
		}
#ifdef __MINGW64__
		glDrawPixels(1, glutBitmapHeight(this->font), GL_RGBA, GL_UNSIGNED_BYTE, cursor);
#else
		glDrawPixels(1, glutBitmapHeight(this->font), GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, cursor);
#endif
		free(cursor);
	}

	void onClick(int button, int status, int x, int y) override {
		int val1 = Window::getSingleton()->getHeight() - y, val2 = this->y + this->height;
		if(x > this->x && x < this->x + this->width && (Window::getSingleton()->getHeight() - y) > this->y &&
				(Window::getSingleton()->getHeight() - y) < this->y + this->height) {
			FocusMediator::getSingleton()->giveFocus(this);
			if(hasFocus) {
				//cursorpos = strlen((char *) this->str);
				this->show_label = false;
			}
		} else {
			FocusMediator::getSingleton()->removeFocus(this);
		}
	}

	void onKeyPress(unsigned char key, int x, int y) override {
		if(hasFocus) {
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
				this->str[strlen((char *) this->str) + 1] = 0;
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
