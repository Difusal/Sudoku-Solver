#pragma once

#include "stdIncludes.h"

class MouseCursor {
public:
	MouseCursor(void);

	bool Update(ALLEGRO_EVENT *ev);
	bool CorrectMousePosition();
	void SetAllReleaseValuesToFalse();

	~MouseCursor(void);


	// Public Variables
	unsigned int x, y;

	bool leftMouseButtonHasJustBeenPressed;
	bool leftMouseButtonWasBeingPressedBefore;
	bool leftMouseButtonPressed;
	bool leftMouseButtonReleased;
	bool leftMouseButtonPressedTwice;

	bool rightMouseButtonPressed;
	bool rightMouseButtonReleased;

private:
	float prevMouseRawX;
	float prevMouseRawY;

	bool possible_double_press;
	unsigned int double_press_counter;
};