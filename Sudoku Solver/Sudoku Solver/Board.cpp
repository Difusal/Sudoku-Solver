#include "Board.h"
#include "Control.h"

Board::Board(void) {
	// initializing variables
	CellSize = Control::GetInstance()->ScreenHeight/15.0;
	BoardWidth = BoardHeight = 9*CellSize;

	// now leaving an equal x and y margin
	BoardX = BoardY = (Control::GetInstance()->ScreenHeight - BoardHeight) / 2.0;

	// initializing selected cell coords ({0, 0} means no cell is selected)
	SelectedCellX = 0;
	SelectedCellY = 0;

	// initializing puzzle vector
	for (unsigned int i = 0; i < 9; i++)
		Puzzle.push_back(vector<int> (9, 0));

	font = al_load_font(CalibriTTF, 25, ALLEGRO_ALIGN_CENTER);
	if (!font) {
		al_show_native_message_box(Control::GetInstance()->GetDisplay(), "Error", "Could not load font file.", "If you have just created the project, create the following folder structure:\n\"yourProjectFolder\\res\\fonts\"\n\nThen place calibri.ttf inside the fonts folder.\nYou can get the font on the internet easily.\n\nPress OK to quit the program.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
}

void Board::Update() {
	// checking if user selected a board cell to edit
	if (Control::GetInstance()->Mouse->leftMouseButtonReleased){
		// checking if mouse click is inside board boundaries
		if (BoardX < Control::GetInstance()->Mouse->x && Control::GetInstance()->Mouse->x < BoardX+BoardWidth &&
			BoardY < Control::GetInstance()->Mouse->y && Control::GetInstance()->Mouse->y < BoardY+BoardHeight) {
				// assigning coordinates of selected cell based on mouse click
				SelectedCellX = BoardX + ((Control::GetInstance()->Mouse->x - BoardX) / CellSize) * CellSize;
				SelectedCellY = BoardY + ((Control::GetInstance()->Mouse->y - BoardY) / CellSize) * CellSize;
		}
		// unselect any selected cell
		else {
			// if any of these variables is set to 0, it means no cell is selected
			SelectedCellX = 0;
			SelectedCellY = 0;
		}
	}
}

void Board::Draw() {
	// white background
	al_draw_filled_rectangle(BoardX, BoardY, BoardX+BoardWidth, BoardY+BoardHeight, White);

	// if any cell is selected, draw different background
	if (SelectedCellX != 0 && SelectedCellY != 0)
		al_draw_filled_rectangle(SelectedCellX, SelectedCellY, SelectedCellX+CellSize, SelectedCellY+CellSize, Yellow);

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

	// drawing numbers on board
	stringstream ss;
	for (unsigned int i = 0; i < 9; i++) {
		for (unsigned int j = 0; j < 9; j++) {
			if (Puzzle[i][j] != 0) {
				// clearing string stream
				ss.str(string());

				// sending number to string
				ss << Puzzle[i][j];

				// printing number
				al_draw_text(font, Black, BoardX + j*CellSize + CellSize/2.0, BoardY + i*CellSize + CellSize/4.0, ALLEGRO_ALIGN_CENTER, ss.str().c_str());
			}
		}
	}
}

Board::~Board(void) {
	// destroying used font
	al_destroy_font(font);
}


bool Board::HasASelectedCell() {
	// if none of the coords is 0, that means there is a selected cell
	if (SelectedCellX != 0 && SelectedCellY != 0)
		return true;

	// return false otherwise
	return false;
}

void Board::SetContentOfSelectedCellTo(unsigned int Number) {
	unsigned int x, y;
	x = (SelectedCellX - BoardX) / CellSize;
	y = (SelectedCellY - BoardY) / CellSize;

	// setting new content of selected cell
	Puzzle[y][x] = Number;
}

void Board::MoveCellSelection( Direction direction ) {
	if (!HasASelectedCell())
		return;

	switch (direction) {
	case RIGHT:
		SelectedCellX += CellSize;
		if (SelectedCellX >= BoardX + BoardWidth)
			SelectedCellX = BoardX + BoardWidth - CellSize;
		break;
	case LEFT:
		SelectedCellX -= CellSize;
		if (SelectedCellX < BoardX)
			SelectedCellX = BoardX;
		break;
	case DOWN:
		SelectedCellY += CellSize;
		if (SelectedCellY >= BoardY + BoardHeight)
			SelectedCellY = BoardY + BoardHeight - CellSize;
		break;
	case UP:
		SelectedCellY -= CellSize;
		if (SelectedCellY < BoardY)
			SelectedCellY = BoardY;
		break;
	}
}