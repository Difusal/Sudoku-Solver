#pragma once

#include "stdIncludes.h"
#include "state.h"

#include "Button.h"

class MenuState: public State {
public:
	virtual void Initialize();
	virtual bool Update( ALLEGRO_EVENT *ev );
	virtual void Draw();
	virtual void Terminate();

private:
	unsigned int titleY;

	unsigned int buttonsSpacing;
	unsigned int buttonsX;
	unsigned int solvePuzzleY, fullScreenY, exitY;

	unsigned int buttonsWidth, buttonsHeight;

	vector<Button*> buttons;
	Button *solvePuzzleButton;
	Button *fullScreenButton;
	Button *exitButton;

	ALLEGRO_FONT *titleFont;
};