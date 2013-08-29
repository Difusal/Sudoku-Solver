#pragma once

#include "stdIncludes.h"
#include "state.h"
#include "globalEnums.h"

#include "Board.h"
#include "Button.h"

class SolverState: public State {
public:
	virtual void Initialize();
	virtual bool Update( ALLEGRO_EVENT *ev );
	virtual void Draw();
	virtual void Terminate();

private:
	SolverStage CurrentStage;
	Board *board;

	vector<Button*> buttons;
	Button *submitButton;

	bool puzzleHasBeenSolved;
	bool showPuzzleSolvedNotification;
};