#include "Board.h"
#include "Control.h"

Board::Board(void) {
	// initializing variables
	CellSize = Control::GetInstance()->ScreenHeight/15.0;
	BoardWidth = BoardHeight = 9*CellSize;

	// now leaving an equal x and y margin
	BoardX = BoardY = (Control::GetInstance()->ScreenHeight - BoardHeight) / 2.0;
}

void Board::Update() {

}

void Board::Draw() {
	// white background
	al_draw_filled_rectangle(BoardX, BoardY, BoardX+BoardWidth, BoardY+BoardHeight, White);

	// complementary guide lines
	for (unsigned int i = 1; i <= 8; i++) {
		// horizontal guides
		al_draw_line(BoardX, i*CellSize + BoardY, BoardX+BoardWidth, i*CellSize + BoardY, Gray, 1.0);

		// vertical guides
		al_draw_line(i*CellSize + BoardX, BoardY, i*CellSize + BoardX, BoardY+BoardHeight, Gray, 1.0);
	}

	// main guide lines
	for (unsigned int i = 1; i <= 3; i++) {
		// horizontal guides
		al_draw_line(BoardX, i*3*CellSize + BoardY, BoardX+BoardWidth, i*3*CellSize + BoardY, Black, 2.0);

		// vertical guides
		al_draw_line(i*3*CellSize + BoardX, BoardY, i*3*CellSize + BoardX, BoardY+BoardHeight, Black, 2.0);
	}

	// limits
	al_draw_rectangle(BoardX, BoardY, BoardX+BoardWidth, BoardY+BoardHeight, Black, 2.0);
}

Board::~Board(void) {

}