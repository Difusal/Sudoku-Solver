#pragma once

#include "stdIncludes.h"

class Board {
public:
	Board(void);
	void Update();
	void Draw();
	~Board(void);

private:
	unsigned int CellSize;
	unsigned int BoardX, BoardY;
	unsigned int BoardWidth, BoardHeight;
};