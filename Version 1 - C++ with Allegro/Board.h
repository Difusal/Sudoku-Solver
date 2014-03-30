#pragma once

#include "stdIncludes.h"
#include "globalEnums.h"

class Board {
public:
	Board(void);
	void Update();
	void Draw();
	~Board(void);

	bool HasASelectedCell();

	unsigned int X() {
		return BoardX;
	}
	unsigned int Y() {
		return BoardY;
	}
	unsigned int Width() {
		return BoardWidth;
	}
	unsigned int Height() {
		return BoardHeight;
	}

	int** GetPuzzle() {
		return Puzzle;
	}

	void UnselectAnyCell() {
		SelectedCellX = 0;
		SelectedCellY = 0;
	}
	void SetContentOfSelectedCellTo(unsigned int Number);
	void MoveCellSelection(Direction direction);

private:
	int** Puzzle;

	unsigned int CellSize;
	unsigned int BoardX, BoardY;
	unsigned int BoardWidth, BoardHeight;

	unsigned int SelectedCellX, SelectedCellY;

	ALLEGRO_FONT *font;
};
