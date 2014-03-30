#pragma once

#include "stdIncludes.h"
#include "state.h"
#include "globalEnums.h"

#include "Board.h"
#include "Button.h"

class SolverState: public State {
public:
	virtual void Initialize();
	virtual bool Update(ALLEGRO_EVENT *ev);
	virtual void Draw();
	virtual void Terminate();

	bool isComplete() {
		return countFilled == 9 * 9;
	}

private:
	unsigned int titleX, titleY;

	SolverStage CurrentStage;
	Board *board;

	vector<Button*> buttons;
	Button *submitButton;

	unsigned int startTime, endTime, elapsedTime;

	bool puzzleHasBeenSolved;
	bool showPuzzleSolvedNotification;

	ALLEGRO_FONT *titleFont;

	int** numbers;
	int countFilled;
	bool columnHasNumber[9][10];
	bool lineHasNumber[9][10];
	bool block3x3HasNumber[3][3][10];

	bool accepts(int j, int i, int n);
	void place(int j, int i, int n);
	void clear(int j, int i);
	bool solve();
};
