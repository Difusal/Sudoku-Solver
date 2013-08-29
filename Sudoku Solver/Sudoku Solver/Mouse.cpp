#include "Mouse.h"
#include "Control.h"

MouseCursor::MouseCursor(void) {
	cout << "Creating mouse cursor..." << endl;

	x = Control::GetInstance()->ScreenWidth;
	y = Control::GetInstance()->ScreenHeight;

	leftMouseButtonHasJustBeenPressed = false;
	leftMouseButtonWasBeingPressedBefore = false;
	leftMouseButtonPressed = false;
	leftMouseButtonReleased = false;

	rightMouseButtonPressed = false;
	rightMouseButtonReleased = false;

	possible_double_press = false;
	double_press_counter = 0;
}

bool MouseCursor::Update(ALLEGRO_EVENT *ev) {
	// initializing return value
	bool draw = false;

	// controlling left mouse button double press
	if (possible_double_press && double_press_counter < FPS*1.5/4)
		double_press_counter++;
	else if (possible_double_press)	{
		possible_double_press = false;
		double_press_counter = 0;
	}

	// tracking mouse position
	if (ev->type == ALLEGRO_EVENT_MOUSE_AXES) {
		x = ev->mouse.x;
		y = ev->mouse.y;

		prevMouseRawX = ev->mouse.x;
		prevMouseRawY = ev->mouse.y;

		draw = true;
	}

	// tracking button presses
	if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (ev->mouse.button &1) {
			//cout << "* Left mouse button pressed *" << endl;
			leftMouseButtonPressed = true;
			leftMouseButtonReleased = false;
			draw = true;
		}
		if (ev->mouse.button &2) {
			//cout << "* Right mouse button pressed *" << endl;
			rightMouseButtonPressed = true;
			rightMouseButtonReleased = false;
			draw = true;
		}
	}

	// tracking button releases
	if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (ev->mouse.button &1) {
			if (possible_double_press) {
				cout << "* Left mouse button released(pressed) twice *" << endl;
				leftMouseButtonPressedTwice = true;
			}
			else
				cout << "* Left mouse button released *" << endl;

			possible_double_press = true;
			leftMouseButtonPressed = false;
			leftMouseButtonReleased = true;

			draw = true;
		}
		if (ev->mouse.button &2) {
			cout << "* Right mouse button released *" << endl;
			rightMouseButtonPressed = false;
			rightMouseButtonReleased = true;

			draw = true;
		}
	}

	// checking if left mouse button has just been pressed
	if (leftMouseButtonPressed && !leftMouseButtonWasBeingPressedBefore)
		leftMouseButtonHasJustBeenPressed = true;
	else
		leftMouseButtonHasJustBeenPressed = false;
	
	return draw;
}

bool MouseCursor::CorrectMousePosition() {
	x = prevMouseRawX;
	y = prevMouseRawY;

	return true;
}

void MouseCursor::SetAllReleaseValuesToFalse() {
	leftMouseButtonWasBeingPressedBefore = leftMouseButtonPressed;
	leftMouseButtonReleased = false;
	leftMouseButtonPressedTwice = false;
	rightMouseButtonReleased = false;
}

MouseCursor::~MouseCursor(void) {
	// nothing to destroy here
}