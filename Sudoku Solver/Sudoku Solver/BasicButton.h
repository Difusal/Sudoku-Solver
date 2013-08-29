#pragma once

#include "stdIncludes.h"

class BasicButton {
public:
	void Update();
	~BasicButton(void);

	unsigned int Width() { return width; }
	unsigned int Height() { return height; }

	bool isPressed() { return beingPressed; }
	bool wasReleased() { return released; }

protected:
	string label;
	unsigned int x, y;
	unsigned int width, height;

	bool beingHovered;
	bool beingPressed;
	bool released;

	ALLEGRO_FONT *font;
};