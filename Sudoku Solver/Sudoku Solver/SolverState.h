#pragma once

#include "stdIncludes.h"
#include "state.h"

#include "Board.h"

class SolverState: public State {
public:
	virtual void Initialize();
	virtual bool Update( ALLEGRO_EVENT *ev );
	virtual void Draw();
	virtual void Terminate();

	void DrawPuzzleBoard();

private:
	Board *board;
};