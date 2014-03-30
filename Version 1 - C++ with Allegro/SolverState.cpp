#include "SolverState.h"
#include "Control.h"

void SolverState::Initialize() {
	CurrentStage = _Editing;
	board = new Board();

	unsigned int buttonX, buttonY;
	buttonX = board->X() + board->Width()
			+ (Control::GetInstance()->ScreenWidth
					- (board->X() + board->Width())) / 2.0;
	buttonY = Control::GetInstance()->ScreenHeight / 2.0;
	submitButton = new Button("Solve this!", buttonX, buttonY, 130, 40);
	buttons.push_back(submitButton);

	puzzleHasBeenSolved = false;
	showPuzzleSolvedNotification = false;

	titleFont = al_load_font(CalibriTTF,
			Control::GetInstance()->ScreenHeight / 10, ALLEGRO_ALIGN_CENTER);
	titleX = board->X() + board->Width()
			+ (Control::GetInstance()->ScreenWidth
					- (board->X() + board->Width())) / 2.0;
	titleY = titleFont->height / 2;

	Control::GetInstance()->Mouse->SetAllReleaseValuesToFalse();
}

bool SolverState::accepts(int j, int i, int n) {
	// check line
	if (lineHasNumber[i][n])
		return false;

	// check column
	if (columnHasNumber[j][n])
		return false;

	// check block
	if (block3x3HasNumber[i / 3][j / 3][n])
		return false;

	return true;
}

void SolverState::place(int j, int i, int n) {
	// updating sudoku
	numbers[i][j] = n;

	// updating line
	lineHasNumber[i][n] = true;

	// updating column
	columnHasNumber[j][n] = true;

	// updating block
	block3x3HasNumber[i / 3][j / 3][n] = true;

	// updating count
	countFilled++;
}

void SolverState::clear(int j, int i) {
	// updating sudoku
	int n = numbers[i][j];
	numbers[i][j] = 0;

	// updating line
	lineHasNumber[i][n] = false;

	// updating column
	columnHasNumber[j][n] = false;

	// updating block
	block3x3HasNumber[i / 3][j / 3][n] = false;

	// updating count
	countFilled--;
}

bool SolverState::solve() {
	Draw();

	if (isComplete())
		return true;

	// start by searching for the cell with less candidates
	int besti = -1;
	int bestj = -1;
	int minNumPossibilities = -1;

	// going through every cell
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// only scan free cells
			if (numbers[i][j] == 0) {
				int numPossibilities = 0;

				// counting possibilities
				for (int n = 1; n <= 9; n++)
					if (accepts(j, i, n))
						numPossibilities++;

				// impossible puzzle
				if (numPossibilities == 0)
					return false;

				// updating data
				if (minNumPossibilities == -1
						|| numPossibilities < minNumPossibilities) {
					besti = i;
					bestj = j;
					minNumPossibilities = numPossibilities;
				}

				// only one possibility
				if (minNumPossibilities == 1)
					break;
			}
		}

		if (minNumPossibilities == 1)
			break;
	}

	// try out every possibility
	for (int n = 1; n <= 9; n++) {
		if (accepts(bestj, besti, n)) {
			// place number
			place(bestj, besti, n);

			// returning if puzzle is solved
			if (solve())
				return true;

			// if not solved, clear wrong possibility
			clear(bestj, besti);
		}
	}

	return false;
}

bool SolverState::Update(ALLEGRO_EVENT *ev) {
	// if ESC was pressed, return to Main Menu
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN
			&& ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		Control::GetInstance()->ChangeState(_Menu);
		return true;
	}

	// if event is a timer event
	if (ev->type == ALLEGRO_EVENT_TIMER) {
		// updating buttons
		for (unsigned int i = 0; i < buttons.size(); i++)
			buttons[i]->Update();
	}

	// describing behavior when a button is pressed
	if (submitButton->wasReleased()) {
		// getting start time
		startTime = (unsigned int) time(NULL);

		// deactivating submit button
		submitButton->Deactivate();

		// unselect any active cell
		board->UnselectAnyCell();

		// getting numbers of the puzzle
		numbers = board->GetPuzzle();

		// initializing data
		countFilled = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				for (int n = 0; n < 10; n++) {
					numbers[i][j] = 0;
					lineHasNumber[i][n] = false;
					columnHasNumber[j][n] = false;
					block3x3HasNumber[i / 3][j / 3][n] = false;
				}
			}
		}

		// changing current state
		CurrentStage = _Solving;
	}

	switch (CurrentStage) {
	// if puzzle has been solved
	case _Resting:
		showPuzzleSolvedNotification = true;
		break;
		// if puzzle has been submitted so it can be solved
	case _Solving:
		if (solve()) {
			// getting end time
			endTime = (unsigned int) time(NULL);

			// getting elapsed time
			elapsedTime = endTime - startTime;

			cout << endl;
			cout << "   .:::::::::::::::::::.   " << endl;
			cout << " .:: ! PUZZLE SOLVED ! ::. " << endl;
			cout << ":::::::::::::::::::::::::::" << endl;
			cout << "___________________________" << endl;
			cout << "> Elapsed time: " << elapsedTime << "s" << endl;

			// changing current state
			CurrentStage = _Resting;
		}

		return true;
		// if puzzle is still being edited
	case _Editing:
		// if event is a timer event
		if (ev->type == ALLEGRO_EVENT_TIMER) {
			// updating board
			board->Update();
		}

		// if there is a selected cell and event is a keyboard event
		if (board->HasASelectedCell() && ev->type == ALLEGRO_EVENT_KEY_DOWN) {
			// moving cell selection
			if (ev->keyboard.keycode == ALLEGRO_KEY_RIGHT)
				board->MoveCellSelection(RIGHT);
			if (ev->keyboard.keycode == ALLEGRO_KEY_LEFT)
				board->MoveCellSelection(LEFT);
			if (ev->keyboard.keycode == ALLEGRO_KEY_DOWN)
				board->MoveCellSelection(DOWN);
			if (ev->keyboard.keycode == ALLEGRO_KEY_UP)
				board->MoveCellSelection(UP);

			// delete cell content (set it to 0)
			if (ev->keyboard.keycode == ALLEGRO_KEY_0
					|| ev->keyboard.keycode == ALLEGRO_KEY_BACKSPACE
					|| ev->keyboard.keycode == ALLEGRO_KEY_DELETE)
				board->SetContentOfSelectedCellTo(0);

			// place respective input to cell
			if (ev->keyboard.keycode == ALLEGRO_KEY_1)
				board->SetContentOfSelectedCellTo(1);
			if (ev->keyboard.keycode == ALLEGRO_KEY_2)
				board->SetContentOfSelectedCellTo(2);
			if (ev->keyboard.keycode == ALLEGRO_KEY_3)
				board->SetContentOfSelectedCellTo(3);
			if (ev->keyboard.keycode == ALLEGRO_KEY_4)
				board->SetContentOfSelectedCellTo(4);
			if (ev->keyboard.keycode == ALLEGRO_KEY_5)
				board->SetContentOfSelectedCellTo(5);
			if (ev->keyboard.keycode == ALLEGRO_KEY_6)
				board->SetContentOfSelectedCellTo(6);
			if (ev->keyboard.keycode == ALLEGRO_KEY_7)
				board->SetContentOfSelectedCellTo(7);
			if (ev->keyboard.keycode == ALLEGRO_KEY_8)
				board->SetContentOfSelectedCellTo(8);
			if (ev->keyboard.keycode == ALLEGRO_KEY_9)
				board->SetContentOfSelectedCellTo(9);
		}
		break;
	}

	return false;
}

void SolverState::Draw() {
	// printing title
	al_draw_text(titleFont, Black, titleX + 2, titleY + 2, ALLEGRO_ALIGN_CENTER,
			"SUDOKU");
	al_draw_text(titleFont, Black, titleX + 2, 2 * titleY + 2,
			ALLEGRO_ALIGN_CENTER, "SOLVER");
	al_draw_text(titleFont, White, titleX, titleY, ALLEGRO_ALIGN_CENTER,
			"SUDOKU");
	al_draw_text(titleFont, White, titleX, 2 * titleY, ALLEGRO_ALIGN_CENTER,
			"SOLVER");

	// drawing board
	board->Draw();

	// drawing buttons
	for (unsigned int i = 0; i < buttons.size(); i++)
		buttons[i]->Draw();

	// if puzzle solved, show a notification
	if (showPuzzleSolvedNotification) {
		al_draw_text(Control::GetInstance()->largeFont, Black,
				Control::GetInstance()->ScreenWidth / 2 + 2,
				Control::GetInstance()->ScreenHeight / 2
						- Control::GetInstance()->largeFont->height / 2 + 2,
				ALLEGRO_ALIGN_CENTER, "PUZZLE SOLVED !");
		al_draw_text(Control::GetInstance()->largeFont, Yellow,
				Control::GetInstance()->ScreenWidth / 2,
				Control::GetInstance()->ScreenHeight / 2
						- Control::GetInstance()->largeFont->height / 2,
				ALLEGRO_ALIGN_CENTER, "PUZZLE SOLVED !");
	}
}

void SolverState::Terminate() {
	// deleting board
	delete board;

	// destroying buttons
	for (unsigned int i = 0; i < buttons.size(); i++)
		delete buttons[i];
	buttons.clear();

	// destroying font
	al_destroy_font(titleFont);
}
