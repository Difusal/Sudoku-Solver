#pragma once

#include "stdIncludes.h"

class Board {
public:
	Board(void);
	void Update();
	void Draw();
	~Board(void);

	bool HasASelectedCell();

	vector<vector<int> > *GetPuzzle();

	void SetContentOfSelectedCellTo(unsigned int Number);

private:
	vector<vector<int> > Puzzle;

	unsigned int CellSize;
	unsigned int BoardX, BoardY;
	unsigned int BoardWidth, BoardHeight;

	unsigned int SelectedCellX, SelectedCellY;

	ALLEGRO_FONT *font;
};