#pragma once

#include "stdIncludes.h"
#include "state.h"

#include "Board.h"
#include "Button.h"

class SolverState: public State {
public:
	virtual void Initialize();
	virtual bool Update( ALLEGRO_EVENT *ev );
	virtual void Draw();
	virtual void Terminate();

	void DrawPuzzleBoard();

private:
	Board *board;

	vector<Button*> buttons;
	Button *submitButton;

	bool puzzleHasBeenSubmitted;
};