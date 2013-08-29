#include "SolverState.h"
#include "Control.h"

void SolverState::Initialize() {
	board = new Board();
}

bool SolverState::Update( ALLEGRO_EVENT *ev ) {

	return false;
}

void SolverState::Draw() {
	// drawing board
	board->Draw();
}

void SolverState::Terminate() {
}

void SolverState::DrawPuzzleBoard() {

}