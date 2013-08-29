#include "BasicButton.h"
#include "Control.h"

void BasicButton::Update() {
	// setting bool variables to their default value
	beingPressed = false;
	beingHovered = false;
	released = false;

	// checking if button is being hovered
	if (x < Control::GetInstance()->Mouse->x && Control::GetInstance()->Mouse->x < x+width &&
		y < Control::GetInstance()->Mouse->y && Control::GetInstance()->Mouse->y < y+height) {
			beingHovered = true;
			
			// checking if button was released
			if (Control::GetInstance()->Mouse->leftMouseButtonPressed) {
				beingPressed = true;
			}

			// checking if button was released
			if (Control::GetInstance()->Mouse->leftMouseButtonReleased) {
				cout << label << " button was released." << endl;
				released = true;
			}
	}
	else {
		beingPressed = false;
		beingHovered = false;
	}
}

BasicButton::~BasicButton(void) {
	al_destroy_font(font);
}