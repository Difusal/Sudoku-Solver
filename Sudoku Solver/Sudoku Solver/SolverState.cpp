#include "SolverState.h"
#include "Control.h"
#include "SudokuFunctions.h"

void SolverState::Initialize() {
	CurrentStage = _Editing;
	board = new Board();

	unsigned int buttonX, buttonY;
	buttonX = board->X()+board->Width() + (Control::GetInstance()->ScreenWidth - (board->X()+board->Width())) / 2.0;
	buttonY = Control::GetInstance()->ScreenHeight / 2.0;
	submitButton = new Button("Submit", buttonX, buttonY, 130, 40);
	buttons.push_back(submitButton);

	puzzleHasBeenSolved = false;
	showPuzzleSolvedNotification = false;

	Control::GetInstance()->Mouse->SetAllReleaseValuesToFalse();
}

bool SolverState::Update( ALLEGRO_EVENT *ev ) {
	// if event is a timer event
	if (ev->type == ALLEGRO_EVENT_TIMER) {
		// updating buttons
		for (Button *button: buttons)
			button->Update();
	}

	// describing behavior when a button is pressed
	if (submitButton->wasReleased()) {
		submitButton->Deactivate();
		CurrentStage = _Solving;
	}

	// keep going
	switch (CurrentStage) {
		// if puzzle has been solved
	case _Resting:
		{
			showPuzzleSolvedNotification = true;
			break;
		}

		// if puzzle has been submitted so it can be solved
	case _Solving:
		{
			// checking if puzzle is solved
			if (PuzzleIsSolved(board->GetPuzzle())) {
				cout << "! ..:: PUZZLE SOLVED ::.. !" << endl;
				board->UnselectAnyCell();
				CurrentStage = _Resting;
				return true;
			}

			// checking if there is not a single empty cell
			// which means that the puzzle is solved incorrectly, since we just checked if the puzzle was solved correctly
			if (NoEmptyCellsLeft(board->GetPuzzle())) {
				cout << "Error" << endl;
				return true;
			}


			// trying to place a number, all the way from 1 to 9
			for (unsigned int numberToBePlaced = 1; numberToBePlaced <= 9; numberToBePlaced++) {

				// going through each module
				for (unsigned int moduleBeingScannedY = 0; moduleBeingScannedY < 3; moduleBeingScannedY++) {
					for (unsigned int moduleBeingScannedX = 0; moduleBeingScannedX < 3; moduleBeingScannedX++) {

						// continuing if current number is already settled on current module being scanned
						if (NumberExistsInModule(numberToBePlaced, moduleBeingScannedX, moduleBeingScannedY, board->GetPuzzle()))
							continue;

						// initializing possibilities vector
						vector<vector<int> > possibilities;
						for (unsigned int i = 0; i < 3; i++)
							possibilities.push_back(vector<int> (3, 0));
						
						// note regarding the possibilities vector:
						//	1 - number can be placed here
						//	0 - number can NOT be placed here

						// if number is not on that module yet, build possibilities vector with the current EMPTY cells
						for (unsigned int i = 0; i < 3; i++) {
							for (unsigned int j = 0; j < 3; j++) {

								// if the cell is empty (aka '0'), put '1' in the possibilities vector
								if (board->GetPuzzle()[3*moduleBeingScannedY + i][3*moduleBeingScannedX + j] == 0)
									possibilities[i][j] = 1;

								// else put a 0
								else			
									possibilities[i][j] = 0;
							}
						}

						// update possibilities vector if the current number is on a RELEVANT line or column
						for (unsigned int i = 0; i < 3; i++) {
							for (unsigned int j = 0; j < 3; j++) {

								// skip if scanning a '0'
								if (possibilities[i][j] == 0)
									continue;

								// if there is a '1' but the current number is already on that LINE, make it a '0'
								if (NumberExistsInLine(numberToBePlaced, 3*moduleBeingScannedY + i, board->GetPuzzle()))
									possibilities[i][j] = 0;

								// if there is a '1' but the current number is already on that COLUMN, make it a '0'
								if (NumberExistsInColumn(numberToBePlaced, 3*moduleBeingScannedX + j, board->GetPuzzle()))
									possibilities[i][j] = 0;
							}
						}

						// debugging block to show content of possibilities vector
						for (unsigned int i = 0; i < 3; i++) {
							for (unsigned int j = 0; j < 3; j++) {
								cout << possibilities[i][j] << "  ";
								if (j == 2)
									cout << endl;
							}
						}
						cout << endl;

						// if there is only one possibility on that module to place the current number, place it! :D
						if (OnlyOnePossibility(possibilities)) {
							PlaceNumberOnTheOnlyPossibleCell(numberToBePlaced, moduleBeingScannedX, moduleBeingScannedY, board->GetPuzzle(), possibilities);
							return true;
						}
					}
				}
			}

			break;
		}

		// if puzzle is still being edited
	case _Editing:
		{
			// if event is a timer event
			if (ev->type == ALLEGRO_EVENT_TIMER) {
				// updating board
				board->Update();
			}

			// if there is a selected cell and event is a keyboard event
			if (board->HasASelectedCell() && ev->type == ALLEGRO_EVENT_KEY_DOWN) {
				// delete cell content (set it to 0)
				if (ev->keyboard.keycode == ALLEGRO_KEY_0 || ev->keyboard.keycode == ALLEGRO_KEY_BACKSPACE || ev->keyboard.keycode == ALLEGRO_KEY_DELETE)
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
	}

	return false;
}

void SolverState::Draw() {
	// drawing board
	board->Draw();

	// drawing buttons
	for (Button *button: buttons)
		button->Draw();

	// if puzzle solved, show a notification
	if (showPuzzleSolvedNotification) {
		al_draw_text(Control::GetInstance()->largeFont, Black, Control::GetInstance()->ScreenWidth/2 + 2, Control::GetInstance()->ScreenHeight/2 - Control::GetInstance()->largeFont->height/2 + 2, ALLEGRO_ALIGN_CENTER, "PUZZLE SOLVED !");
		al_draw_text(Control::GetInstance()->largeFont, Yellow, Control::GetInstance()->ScreenWidth/2, Control::GetInstance()->ScreenHeight/2 - Control::GetInstance()->largeFont->height/2, ALLEGRO_ALIGN_CENTER, "PUZZLE SOLVED !");
	}
}

void SolverState::Terminate() {
}