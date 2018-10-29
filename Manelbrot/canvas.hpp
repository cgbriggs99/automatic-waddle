/*
 * canvas.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: cgbri
 */

#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include "./defs.hpp"
#include "./visible.hpp"
#include "window.hpp"
#include "update_command.hpp"
#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>
#include <GL/freeglut.h>

class Canvas : public Visible, public UpdateCommandObserver {
private:
	color_t *pixels;
	int x, y, width, height;
	color_t curr_selection;
	bool frame;
public:
	virtual void paint() = 0;

	Canvas(int x, int y, int width, int height, bool frame = false) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->pixels = new color_t[width * height];
		this->curr_selection = {255, 0, 0, 0};
		this->needs_update = true;
		UpdateCommandReceiver::getSingleton()->registerObserver(this);
		this->frame = frame;
	}

	virtual ~Canvas() {
		delete(pixels);
	}

	void draw() override {
		Window *w = Window::getSingleton();
		if(needs_update) {
			this->clear();
			this->paint();
			if(frame) {
				drawRectangle(makeColor4i(0, 0, 0, 255), 0, 0, width, height);
			}
			needs_update = false;
		}
		glPushMatrix();
		glLoadIdentity();
		float check = 1 - (2.0f * (y + height)) / w->getHeight();
		glRasterPos2f((2.0f * (x)) / w->getWidth() - 1, (2.0f * y) / w->getHeight() - 1);
#ifdef __MINGW64__
		glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#else
		glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
#endif
		glPopMatrix();
	}

	void setPixel(int x, int y, color_t color) {
		pixels[x + y * width] = alphaComposite(color, pixels[x + y * width]);
	}

	void setPixel4f(int x, int y, float r, float g, float b, float a) {
		color_t color;
		color.a = (unsigned char) (255 * a);
		color.b = (unsigned char) (255 * b);
		color.g = (unsigned char) (255 * g);
		color.r = (unsigned char) (255 * r);

		pixels[x + y * width] = alphaComposite(color, pixels[x + y * width]);
	}

	void clear() {
		for(int i = 0; i < width * height; i++) {
			pixels[i] = {0, 0, 0, 0};
		}
	}

	void pickColor(color_t color) {
		this->curr_selection = color;
	}

	void pickColor4f(float r, float g, float b, float a) {
		this->curr_selection.a = (unsigned char) (255 * a);
		this->curr_selection.b = (unsigned char) (255 * b);
		this->curr_selection.g = (unsigned char) (255 * g);
		this->curr_selection.r = (unsigned char) (255 * r);
	}

	void pickColor4i(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		this->curr_selection.a = a;
		this->curr_selection.b = b;
		this->curr_selection.g = g;
		this->curr_selection.r = r;
	}

	void setColor(color_t color) {
		for(int i = 0; i < width * height; i++) {
			pixels[i] = color;
		}
	}

	void translate2i(int dx, int dy) {
		color_t *temp = (color_t *) calloc(this->width * this->height, sizeof(color_t));
		memcpy(temp, this->pixels, this->width * this->height * sizeof(color_t));

		this->clear();
		for(int i = (dx < 0)? 0: dx; i < (dx < 0)? this->width + dx: this->width; i++) {
			for(int j = (dy < 0)? 0: dy; j < (dy < 0)? this->height + dx: this->height; j++) {
				this->pixels[i + j * this->width] = temp[i - dx + (j - dy) * this->width];
			}
		}
		free(temp);
	}

	void setColor4f(float r, float g, float b, float a) {
		for(int i = 0; i < width * height; i++) {
			pixels[i].a = (unsigned char) (255 * a);
			pixels[i].r = (unsigned char) (255 * r);
			pixels[i].g = (unsigned char) (255 * g);
			pixels[i].b = (unsigned char) (255 * b);
		}
	}

	void setColor4i(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		for(int i = 0; i < width * height; i++) {
			pixels[i].a = a;
			pixels[i].r = r;
			pixels[i].g = g;
			pixels[i].b = b;
		}
	}

	void overlayColor(color_t color) {
		for(int i = 0; i < width * height; i++) {
			pixels[i] = alphaComposite(color, pixels[i]);
		}
	}

	void overlayColor4f(float r, float g, float b, float a) {
		color_t color;
		color.a = (unsigned char) (255 * a);
		color.g = (unsigned char) (255 * g);
		color.b = (unsigned char) (255 * b);
		color.r = (unsigned char) (255 * r);
		for(int i = 0; i < width * height; i++) {
			pixels[i] = alphaComposite(color, pixels[i]);
		}
	}

	void overlayColor4i(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		color_t color;
		color.a = a;
		color.b = b;
		color.g = g;
		color.r = r;
		for(int i = 0; i < width * height; i++) {
			pixels[i] = alphaComposite(color, pixels[i]);
		}
	}

	void drawRectangle(color_t color, int x, int y, int width, int height) {
		for(int i = 0; i < width; i++) {
			pixels[x + i + y * this->width] =
					alphaComposite(color, pixels[x + i + y * this->width]);
			pixels[x + i + (y + height - 1) * this->width] =
					alphaComposite(color, pixels[x + i + (y + height - 1) * this->width]);
		}

		for(int i = 0; i < height; i++) {
			pixels[x + (y + i) * this->width] =
					alphaComposite(color, pixels[x + (y + i) * this->width]);
			pixels[x + width - 1 + (y + i) * this->width] =
					alphaComposite(color, pixels[x + width - 1 + (y + i) * this->width]);
		}
	}

	void drawRectangle(int x, int y, int width, int height) {
		for(int i = 0; i < width; i++) {
			pixels[x + i + y * this->width] =
					alphaComposite(this->curr_selection, pixels[x + i + y * this->width]);
			int check = x + i + (y - height + 1) * this->width;
			pixels[x + i + (y + height - 1) * this->width] =
					alphaComposite(this->curr_selection, pixels[x + i + (y + height - 1) * this->width]);
		}

		for(int i = 0; i < height; i++) {
			pixels[x + (y + i) * this->width] =
					alphaComposite(this->curr_selection, pixels[x + (y + i) * this->width]);
			pixels[x + width - 1 + (y + i) * this->width] =
					alphaComposite(this->curr_selection, pixels[x + width - 1 + (y + i) * this->width]);
		}
	}

	void antialias() {
		color_t *old = (color_t *) calloc(width * height, sizeof(color_t));
		memcpy(old, pixels, width * height * sizeof(color_t));
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				color_t sum;
				sum.a = 0;
				sum.b = 0;
				sum.g = 0;
				sum.r = 0;
				if(i != 0) {
					sum = alphaComposite(sum, old[i - 1 + j * width]);
					if(j != 0) {
						sum = alphaComposite(sum, old[i - 1 + (j - 1) * width]);
					}
					if(j != height - 1) {
						sum = alphaComposite(sum, old[i - 1 + (j + 1) * width]);
					}
				}
				if(i != width - 1) {
					sum = alphaComposite(sum, old[i - 1 + j * width]);
					if(j != 0) {
						sum = alphaComposite(sum, old[i + 1 + (j - 1) * width]);
					}
					if(j != height - 1) {
						sum = alphaComposite(sum, old[i + 1 + (j + 1) * width]);
					}
				}
				if(j != 0) {
					sum = alphaComposite(sum, old[i + (j - 1) * width]);
				}
				if(j != height - 1) {
					sum = alphaComposite(sum, old[i + (j + 1) * width]);
				}
				sum.a /= 3;
				pixels[i + j * width] = alphaComposite(sum, pixels[i + j * width]);
			}
		}
		free(old);
	}

	int getWidth() {
		return (this->width);
	}

	int getHeight() {
		return (this->height);
	}

	int getX() {
		return (this->x);
	}

	int getY() {
		return (this->y);
	}
};

#endif /* CANVAS_HPP_ */
