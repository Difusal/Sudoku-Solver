#pragma once

#include "stdIncludes.h"

struct Coords {
	unsigned int x;
	unsigned int y;
};

bool PuzzleIsSolved(const vector<vector<int> > &Puzzle) {
	// calculating line and columns sum
	for (unsigned int i = 0; i < 9; i++) {
		// initializing sum values on each cycle start
		unsigned int lineSum = 0;
		unsigned int columnSum = 0;

		for (unsigned int j = 0; j < 9; j++) {
			lineSum += Puzzle[i][j];
			columnSum += Puzzle[j][i];

			// now analyzing sum results
			if (i == 8 && j == 8)
				// if the line or column sum is different than 45 (1+2+...+8+9), return false (puzzle not finished)
					if (lineSum != 45 || columnSum != 45)
						return 0;
		}
	}

	// calculating module sums
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			// initializing module sum before checking each module
			unsigned int moduleSum = 0;

			// computing module sum
			for (unsigned int y = 0; y < 3; y++) {
				for (unsigned int x = 0; x < 3; x++) {
					moduleSum += Puzzle[i*3 + y][j*3 + x];
				}
			}

			// if the sum of a module is different than 45, return false (puzzle not finished)
			if (moduleSum != 45)
				return 0;
		}
	}

	// if everything before this is OK: Puzzle solved: return true (finished)
	return 1;
}

bool NoEmptyCellsLeft(const vector<vector<int> > &Puzzle) {
	// computing module sum
	for (unsigned int i = 0; i < 9; i++) {
		for (unsigned int j = 0; j < 9; j++) {
			// if an empty cell is found, return false
			if (Puzzle[i][j] == 0)
				return false;
		}
	}

	// if no empty cells found, return true
	return true;
}

bool LineIsComplete(unsigned int Line, const vector<vector<int> > &Puzzle) {
	for (unsigned int i = 0; i < 9; i++) {
		// if an empty cell found, return false (line is not completed)
		if (Puzzle[Line][i] == 0)
			return false;
	}

	// return true otherwise
	return true;
}

bool ColumnIsComplete(unsigned int Column, const vector<vector<int> > &Puzzle) {
	for (unsigned int i = 0; i < 9; i++) {
		// if an empty cell found, return false (column is not completed)
		if (Puzzle[i][Column] == 0)
			return false;
	}

	// return true otherwise
	return true;
}

bool NumberExistsInLine(unsigned int Number, unsigned int Line, const vector<vector<int> > &Puzzle) {
	for (unsigned int j = 0; j < 9; j++)
		// if number found, return true (Number exists)
			if (Puzzle[Line][j] == Number)
				return true;

	// if no match found, return false (Number does not exist)
	return false;
}

bool NumberExistsInColumn(unsigned int Number, unsigned int Column, const vector<vector<int> > &Puzzle) {
	for (unsigned int i = 0; i < 9; i++)
		// if number found, return true (Number exists)
			if (Puzzle[i][Column] == Number)
				return true;

	// if no match found, return false (Number does not exist)
	return false;
}

bool NumberExistsInModule(unsigned int Number, unsigned int ModuleX, unsigned int ModuleY, const vector<vector<int> > &Puzzle) {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			// if there is a match, return true (number exists)
			if (Number == Puzzle[3*ModuleY + i][3*ModuleX + j])
				return true;
		}
	}

	// if no match found, return false
	return false;
}

bool NumberCanBePlacedInCell(unsigned int Number, unsigned int Line, unsigned int Column, const vector<vector<int> > &Puzzle) {
	if (NumberExistsInLine(Number, Line, Puzzle))
		return false;

	if (NumberExistsInColumn(Number, Column, Puzzle))
		return false;

	if (NumberExistsInModule(Number, Column/3, Line/3, Puzzle))
		return false;

	// if all conditions are met, number can be placed
	return true;
}

bool OnlyOnePossibility(const vector<vector<int> > &Possibilities) {
	// initializing counter of possible places to put the current number
	unsigned int possiblePlacementsCounter = 0;

	// going through the entire module being scanned
	for (unsigned int i = 0; i < Possibilities.size(); i++) {
		for (unsigned int j = 0; j < Possibilities[i].size(); j++) {
			// if '1' (aka possible place) is found, increment counter
			if (Possibilities[i][j] == 1)
				possiblePlacementsCounter++;

			// if counter is already bigger than 1, function can already return false: which means there is still more than one possibility
			if (possiblePlacementsCounter > 1)
				return false;
		}
	}

	// making sure that after the entire scan the counter only counter 1 occurrence. if so return true
	if (possiblePlacementsCounter == 1)
		return true;

	// return false otherwise
	return false;
}

void PlaceNumberOnTheOnlyPossibleCell(unsigned int Number, unsigned int ModuleX, unsigned int ModuleY, vector<vector<int> > &Puzzle, const vector<vector<int> > &Possibilities) {
	// initializing counter of possible places to put the current number
	unsigned int possiblePlacementsCounter = 0;

	unsigned int possibleCellX, possibleCellY;

	// going through the entire module being scanned
	for (unsigned int i = 0; i < Possibilities.size(); i++) {
		for (unsigned int j = 0; j < Possibilities[i].size(); j++) {

			// if '1' (aka possible place) is found
			if (Possibilities[i][j] == 1) {

				// increment counter
				possiblePlacementsCounter++;

				// and save coords
				possibleCellX = j;
				possibleCellY = i;
			}

			// if counter is already bigger than 1, function can already return: which means there is still more than one possibility...
			if (possiblePlacementsCounter > 1)
				return;
		}
	}

	// if counter is equal to 1, place number there
	if (possiblePlacementsCounter == 1)
		Puzzle[3*ModuleY + possibleCellY][3*ModuleX + possibleCellX] = Number;
}

/*
Coords GetCoordsOfNumberInColumn(unsigned int Number, unsigned int Column, const vector<vector<int> > &Puzzle) {
	// declaring struct that holds the Number coords
	Coords solution;

	// scan the entire column
	for (unsigned int i = 0; i < 9; i++) {
		// if Number found, return solution with the respective coords
		if (Puzzle[i][Column] == Number) {
			// defining solution x and y coords
			solution.x = Column;
			solution.y = i;

			break;
		}
	}

	// returning solution
	return solution;
}

Coords GetCoordsOfNumberInLine(unsigned int Number, unsigned int Line, const vector<vector<int> > &Puzzle) {
	// declaring struct that holds the Number coords
	Coords solution;

	// scan the entire line
	for (unsigned int j = 0; j < 9; j++) {
		// if Number found, return solution with the respective coords
		if (Puzzle[Line][j] == Number) {
			// defining solution x and y coords
			solution.x = j;
			solution.y = Line;

			break;
		}
	}

	// returning solution
	return solution;
}
*/

vector<unsigned int> GetMissingNumbersOnLine(unsigned int Line, const vector<vector<int> > &Puzzle) {
	// initializing vector with all numbers from 1 to 9
	vector<unsigned int> missingNumbers;
	for (int i = 1; i <= 9; i++)
		missingNumbers.push_back(i);

	// going through line
	for (unsigned int i = 0; i < 9; i++) {

		// if a number was found
		if (Puzzle[Line][i] != 0) {

			// go through missing numbers vector
			for (unsigned int j = 0; j < missingNumbers.size(); j++) {

				// and if there is a match
				if (missingNumbers[j] == Puzzle[Line][i])

					// remove it from missing numbers vector
						missingNumbers.erase(missingNumbers.begin() + j);
			}
		}
	}

	// returning vector
	return missingNumbers;
}

vector<unsigned int> GetMissingNumbersOnColumn(unsigned int Column, const vector<vector<int> > &Puzzle) {
	// initializing vector with all numbers from 1 to 9
	vector<unsigned int> missingNumbers;
	for (int i = 1; i <= 9; i++)
		missingNumbers.push_back(i);

	// going through line
	for (unsigned int i = 0; i < 9; i++) {

		// if a number was found
		if (Puzzle[i][Column] != 0) {

			// go through missing numbers vector
			for (unsigned int j = 0; j < missingNumbers.size(); j++) {

				// and if there is a match
				if (missingNumbers[j] == Puzzle[i][Column])

					// remove it from missing numbers vector
						missingNumbers.erase(missingNumbers.begin() + j);
			}
		}
	}

	// returning vector
	return missingNumbers;
}

vector<unsigned int> GetPositionsOfEmptyCellsOnLine(unsigned int Line, const vector<vector<int> > &Puzzle) {
	// declaring vector that will hold the position of the empty cells
	vector<unsigned int> emptyCellsPostions;

	// going through line
	for (unsigned int i = 0; i < 9; i++) {

		// if an empty cell was found
		if (Puzzle[Line][i] == 0) {

			// add current scanning position to empty cells positions vector
			emptyCellsPostions.push_back(i);
		}
	}

	// returning vector
	return emptyCellsPostions;
}

vector<unsigned int> GetPositionsOfEmptyCellsOnColumn(unsigned int Column, const vector<vector<int> > &Puzzle) {
	// declaring vector that will hold the position of the empty cells
	vector<unsigned int> emptyCellsPostions;

	// going through line
	for (unsigned int i = 0; i < 9; i++) {

		// if an empty cell was found
		if (Puzzle[i][Column] == 0) {

			// add current scanning position to empty cells positions vector
			emptyCellsPostions.push_back(i);
		}
	}

	// returning vector
	return emptyCellsPostions;
}