package logic;

import gui.BoardPanel;

public class Solver {
	private int[][] numbers;

	private int countFilled;
	private boolean columnHasNumber[][];
	private boolean lineHasNumber[][];
	private boolean block3x3HasNumber[][][];

	private BoardPanel boardPanel;

	public Solver(BoardPanel boardPanel) {
		this.numbers = boardPanel.getNumbers();

		columnHasNumber = new boolean[9][10];
		lineHasNumber = new boolean[9][10];
		block3x3HasNumber = new boolean[3][3][10];

		this.boardPanel = boardPanel;
	}

	public boolean start() {
		initializeData();
		return solve();
	}

	private boolean isComplete() {
		return countFilled == 9 * 9;
	}

	private boolean accepts(int j, int i, int n) {
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

	private void place(int j, int i, int n) {
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

	private void clear(int j, int i) {
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

	private boolean solve() {
		boardPanel.repaint();

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

	private void initializeData() {
		// initializing data
		countFilled = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				for (int n = 0; n < 10; n++) {
					lineHasNumber[i][n] = false;
					columnHasNumber[j][n] = false;
					block3x3HasNumber[i / 3][j / 3][n] = false;
				}
			}
		}

		// updating data
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (numbers[i][j] != 0) {
					int n = numbers[i][j];
					numbers[i][j] = n;
					lineHasNumber[i][n] = true;
					columnHasNumber[j][n] = true;
					block3x3HasNumber[i / 3][j / 3][n] = true;
					countFilled++;
				}
			}
		}
	}
}
