#include "SolverState.h"
#include "Control.h"
#include "SudokuFunctions.h"

void SolverState::Initialize() {
	CurrentStage = _Editing;
	board = new Board();

	unsigned int buttonX, buttonY;
	buttonX = board->X()+board->Width() + (Control::GetInstance()->ScreenWidth - (board->X()+board->Width())) / 2.0;
	buttonY = Control::GetInstance()->ScreenHeight / 2.0;
	submitButton = new Button("Solve this!", buttonX, buttonY, 130, 40);
	buttons.push_back(submitButton);

	bruteForceID = -1;
	bruteForceDataMaxSizeAllowed = 1;

	iterationCounter = 0;
	samePuzzleCycleCounter = 0;
	sameNumberToBePlacedCounter = 0;
	numberToBePlaced = 1;

	gameIsFrozen = false;
	checkingIfFrozen = false;
	puzzleHasBeenSolved = false;
	showPuzzleSolvedNotification = false;

	titleFont = al_load_font(CalibriTTF, Control::GetInstance()->ScreenHeight/10, ALLEGRO_ALIGN_CENTER);
	titleX = board->X()+board->Width() + (Control::GetInstance()->ScreenWidth - (board->X()+board->Width()))/2.0;
	titleY = titleFont->height/2;

	Control::GetInstance()->Mouse->SetAllReleaseValuesToFalse();
}

void SolverState::CheckIfPuzzleSolvingIsFrozen() {
	// checking if solver is frozen and needs to use attempts in order to complete puzzle
	if (!checkingIfFrozen && puzzleClone == board->GetPuzzle())
		samePuzzleCycleCounter++;
	// if the puzzle isn't changed for 20 iterations, check if it is frozen
	if (!checkingIfFrozen && samePuzzleCycleCounter > 20) {
		cout << "Checking if puzzle is frozen..." << endl;
		samePuzzleCycleCounter = 0;

		// check if frozen
		checkingIfFrozen = true;
		sameNumberToBePlacedCounter = 0;
		// save number that is trying to be placed
		numberTempSave = numberToBePlaced;
	}
	if (checkingIfFrozen && numberToBePlaced == numberTempSave) {
		sameNumberToBePlacedCounter++;
		if (sameNumberToBePlacedCounter > 10) {
			checkingIfFrozen = false;
			gameIsFrozen = true;
		}
	}
}

bool SolverState::CheckIfPuzzleIsSolved() {
	// checking if puzzle is solved
	if (PuzzleIsSolved(board->GetPuzzle())) {
		// getting end time
		endTime = (unsigned int)time(NULL);

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

		return true;
	}

	return false;
}

void SolverState::CheckForAnyExistingErrors() {
	// checking for errors in each line
	for (unsigned int line = 0; line < 9; line++) {

		// if there is only one empty cell on that line
		if (GetPositionsOfEmptyCellsOnLine(line, board->GetPuzzle()).size() == 1) {

			// but the missing number cannot be placed there, something is wrong!
			if (!NumberCanBePlacedInCell(GetMissingNumbersOnLine(line, board->GetPuzzle())[0], line, GetPositionsOfEmptyCellsOnLine(line, board->GetPuzzle())[0], board->GetPuzzle())) {
				cout << "! ERROR DETECTED !" << endl;
				AttemptNextCandidate();
				return;
			}
		}
	}
	// checking for errors in each column
	for (unsigned int column = 0; column < 9; column++) {

		// if there is only one empty cell on that line
		if (GetPositionsOfEmptyCellsOnColumn(column, board->GetPuzzle()).size() == 1) {

			// but the missing number cannot be placed there, something is wrong!
			if (!NumberCanBePlacedInCell(GetMissingNumbersOnColumn(column, board->GetPuzzle())[0], column, GetPositionsOfEmptyCellsOnColumn(column, board->GetPuzzle())[0], board->GetPuzzle())) {
				cout << "! ERROR DETECTED !" << endl;
				AttemptNextCandidate();
				return;
			}
		}
	}
}

void SolverState::AttemptNextCandidate() {
	cout << "! ...::: TRYING NEXT CANDIDATE :::... !" << endl;

	// trying the other candidate of the cell being brute forced
	bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].candidateBeingPlaced++;

	// if for this cell, there are no more candidates
	if (bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].candidateBeingPlaced >= bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].candidates.size()) {
		// restarting this value
		bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].candidateBeingPlaced = 0;

		// restoring puzzle state (emptying cell basically?)
		board->GetPuzzle() = bruteforceData[bruteForceID].puzzleBackup;

		// bruteforce another cell
		bruteforceData[bruteForceID].emptyCellBeingBruteForced++;

		// if there are no more cells
		if (bruteforceData[bruteForceID].emptyCellBeingBruteForced >= bruteforceData[bruteForceID].emptyCells.size()) {
			bruteforceData[bruteForceID].emptyCellBeingBruteForced = 0;
			bruteforceData.erase(bruteforceData.begin() + bruteForceID);
			bruteForceID--;
			if (bruteForceID < 0) {
				cout << "Starting New Level Of Backup" << endl;
				bruteForceDataMaxSizeAllowed++;
			}
			else
				AttemptNextCandidate();
		}
	}
}

void SolverState::ClearBruteForceData() {
	// clearing previous restore points
	bruteforceData.clear();
	bruteForceID = -1;
	bruteForceDataMaxSizeAllowed = 1;
}

bool SolverState::Update( ALLEGRO_EVENT *ev ) {
	// if ESC was pressed, return to Main Menu
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN && ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		Control::GetInstance()->ChangeState(_Menu);
		return true;
	}

	// if event is a timer event
	if (ev->type == ALLEGRO_EVENT_TIMER) {
		// updating buttons
		for (Button *button: buttons)
			button->Update();
	}

	// describing behavior when a button is pressed
	if (submitButton->wasReleased()) {
		// getting start time
		startTime = (unsigned int)time(NULL);

		// deactivating submit button
		submitButton->Deactivate();

		// unselect any active cell
		board->UnselectAnyCell();

		// changing current state
		CurrentStage = _Solving;

		ClearBruteForceData();
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
			// incrementing iteration counter
			iterationCounter++;
			cout << "Iteration: " << iterationCounter << "  " << "Bruteforce ID: " << bruteForceID << endl;

			// if CheckIfPuzzleIsSolved() returns true, puzzle is solved. so return true
			if (CheckIfPuzzleIsSolved())
				return true;

			CheckForAnyExistingErrors();
			CheckIfPuzzleSolvingIsFrozen();

			if (gameIsFrozen) {
				gameIsFrozen = false;
				cout << "============= Game is frozen =============" << endl;

				// creating new restore point
				if (bruteForceID == -1 ||
					(board->GetPuzzle() != bruteforceData[bruteForceID].puzzleBackup &&
					bruteforceData.size() < bruteForceDataMaxSizeAllowed)) {
					bruteForceID++;

					BruteforcingData tempData;
					// saving current puzzle state
					tempData.puzzleBackup = board->GetPuzzle();
					// saving empty cells sorted by candidate numbers
					for (unsigned int line = 0; line < 9; line++) {
						for (unsigned int column = 0; column < 9; column++) {
							// skip if cell is not empty
							if (board->GetPuzzle()[line][column] != 0)
								continue;

							CellDetails tempCellDetails;
							tempCellDetails.candidates = GetCellCandidates(line, column, board->GetPuzzle());
							tempCellDetails.coords.x = column;
							tempCellDetails.coords.y = line;
							tempCellDetails.candidateBeingPlaced = 0;

							tempData.emptyCells.push_back(tempCellDetails);
							tempData.emptyCellBeingBruteForced = 0;
						}
					}
					// sorting emptyCells vector by number of candidates (starting with the one which has less candidates)
					sort(tempData.emptyCells.begin(), tempData.emptyCells.end(), [](CellDetails a, CellDetails b) { return a.candidates.size() < b.candidates.size(); });

					// pushing entire data to bruteforceData
					bruteforceData.push_back(tempData);
				}
				else {
					AttemptNextCandidate();
				}

				// trying to solve puzzle by brute forcing candidates right now
				unsigned int temp_x, temp_y;
				temp_x = bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].coords.x;
				temp_y = bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].coords.y;
				board->GetPuzzle()[temp_y][temp_x] = bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].candidates[bruteforceData[bruteForceID].emptyCells[bruteforceData[bruteForceID].emptyCellBeingBruteForced].candidateBeingPlaced];
			}

			// keeping record of puzzle current state before the iteration makes changes to it
			puzzleClone = board->GetPuzzle();

			// trying to complete a line
			for (unsigned int line = 0; line < 9; line++) {

				// skip if scanning line is already complete
				if (LineIsComplete(line, board->GetPuzzle()))
					continue;

				// getting vector of missing numbers on that line
				vector<unsigned int> missingNumbers = GetMissingNumbersOnLine(line, board->GetPuzzle());

				// getting vector of empty cells positions on that line
				vector<unsigned int> emptyCellsPositions = GetPositionsOfEmptyCellsOnLine(line, board->GetPuzzle());

				// checking if there is only one missing number available to be placed for each of the empty cells
				// going through each empty cell on the line
				for (unsigned int i = 0; i < emptyCellsPositions.size(); i++) {

					// initializing vector
					vector<unsigned int> positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell;

					// going through missing numbers on that line
					for (unsigned int j = 0; j < missingNumbers.size(); j++) {

						// if current missing number being scanned can be placed on that cell
						if (NumberCanBePlacedInCell(missingNumbers[j], line, emptyCellsPositions[i], board->GetPuzzle()))
							// add it to this vector
							positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell.push_back(j);
					}

					// if at the end, on this cell, there's only one possibility, pace number there
					if (positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell.size() == 1)
						board->GetPuzzle()[line][emptyCellsPositions[i]] = missingNumbers[positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell[0]];
				}
			}


			// trying to complete column
			for (unsigned int column = 0; column < 9; column++) {

				// skip if scanning line is already complete
				if (ColumnIsComplete(column, board->GetPuzzle()))
					continue;

				// getting vector of missing numbers on that line
				vector<unsigned int> missingNumbers = GetMissingNumbersOnColumn(column, board->GetPuzzle());

				// getting vector of empty cells positions on that line
				vector<unsigned int> emptyCellsPositions = GetPositionsOfEmptyCellsOnColumn(column, board->GetPuzzle());

				// checking if there is only one missing number available to be placed for each of the empty cells
				// going through each empty cell on the column
				for (unsigned int i = 0; i < emptyCellsPositions.size(); i++) {

					// initializing vector
					vector<unsigned int> positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell;

					// going through missing numbers on that column
					for (unsigned int j = 0; j < missingNumbers.size(); j++) {

						// if current missing number being scanned can be placed on that cell
						if (NumberCanBePlacedInCell(missingNumbers[j], emptyCellsPositions[i], column, board->GetPuzzle()))
							// add it to this vector
								positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell.push_back(j);
					}

					// if at the end, on this cell, there's only one possibility, pace number there
					if (positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell.size() == 1)
						board->GetPuzzle()[emptyCellsPositions[i]][column] = missingNumbers[positionsOfMissingNumbersElementsThatCanBePlacedOnCurrentCell[0]];
				}
			}


			// trying to place a number, all the way from 1 to 9
			if (numberToBePlaced > 9)
				numberToBePlaced = 1;

			// this bool tells if the numberToBePlaced was actually placed in any cell
			bool numberWasPlacedSomewhere = false;

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

					// if there is only one possibility on that module to place the current number, finally place it! :D
					if (OnlyOnePossibility(possibilities)) {
						PlaceNumberOnTheOnlyPossibleCell(numberToBePlaced, moduleBeingScannedX, moduleBeingScannedY, board->GetPuzzle(), possibilities);
						numberWasPlacedSomewhere = true;
					}
				}
			}

			// if number was placed in any cell, decrement numberToBePlaced so the same number
			// can be used in the next cycle again. If we placed one or more now, there is a
			// bigger chance of placing the number used in one of the first modules now.
			// This won't solve any special case, but can improve time taken to solve a puzzle.
			if (numberWasPlacedSomewhere)
				numberToBePlaced--;

			// incrementing number to be placed
			numberToBePlaced++;

			return true;
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
	// printing title
	al_draw_text(titleFont, Black, titleX + 2, titleY + 2, ALLEGRO_ALIGN_CENTER, "SUDOKU");
	al_draw_text(titleFont, Black, titleX + 2, 2*titleY + 2, ALLEGRO_ALIGN_CENTER, "SOLVER");
	al_draw_text(titleFont, White, titleX, titleY, ALLEGRO_ALIGN_CENTER, "SUDOKU");
	al_draw_text(titleFont, White, titleX, 2*titleY, ALLEGRO_ALIGN_CENTER, "SOLVER");

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
	// deleting board
	delete board;

	// destroying buttons
	for (Button *button: buttons)
		delete button;
	buttons.clear();

	// destroying font
	al_destroy_font(titleFont);
}