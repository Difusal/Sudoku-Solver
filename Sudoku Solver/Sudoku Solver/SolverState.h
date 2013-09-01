#pragma once

#include "stdIncludes.h"
#include "state.h"
#include "globalEnums.h"

#include "Board.h"
#include "Button.h"

struct Coords {
	unsigned int x;
	unsigned int y;
};

struct CellDetails {
	Coords coords;
	vector<unsigned int> candidates;
	unsigned int candidateBeingPlaced;
};

struct BruteforcingData {
	vector<vector<int> > puzzleBackup;
	vector<CellDetails> emptyCells;
	unsigned int emptyCellBeingBruteForced;
};


class SolverState: public State {
public:
	virtual void Initialize();
	bool CheckIfPuzzleIsSolved();
	void CheckForAnyExistingErrors();
	void CheckIfPuzzleSolvingIsFrozen();
	void AttemptNextCandidate();
	void ClearBruteForceData();
	virtual bool Update( ALLEGRO_EVENT *ev );
	virtual void Draw();
	virtual void Terminate();

private:
	unsigned int titleX, titleY;

	SolverStage CurrentStage;
	Board *board;
	vector<vector<int> > puzzleClone;

	int bruteForceID;
	unsigned int bruteForceDataMaxSizeAllowed;
	vector<BruteforcingData> bruteforceData;

	vector<Button*> buttons;
	Button *submitButton;

	unsigned int iterationCounter;
	unsigned int samePuzzleCycleCounter, sameNumberToBePlacedCounter, numberTempSave;
	unsigned int numberToBePlaced;
	unsigned int startTime, endTime, elapsedTime;

	bool gameIsFrozen, checkingIfFrozen;
	bool puzzleHasBeenSolved;
	bool showPuzzleSolvedNotification;

	ALLEGRO_FONT *titleFont;
};