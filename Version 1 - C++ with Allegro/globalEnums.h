#pragma once

enum Direction {
	RIGHT,
	LEFT,
	DOWN,
	UP
};

enum ControlState {
	_Menu,
	_Solver
};

enum SolverStage {
	_Editing,
	_Solving,
	_Resting
};

enum TimerType {
	_RegularTimer
};