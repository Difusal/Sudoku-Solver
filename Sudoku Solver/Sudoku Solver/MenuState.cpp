#include "MenuState.h"
#include "Control.h"

void MenuState::Initialize() {
	// initializing variables
	titleY = Control::GetInstance()->ScreenHeight/12.0;

	buttonsSpacing = Control::GetInstance()->ScreenHeight/6.0;

	buttonsX = Control::GetInstance()->ScreenWidth/2;
	fullScreenY = Control::GetInstance()->ScreenHeight/2.0;
	solvePuzzleY = fullScreenY - buttonsSpacing;
	exitY = fullScreenY + buttonsSpacing;

	buttonsWidth = Control::GetInstance()->ScreenWidth/3.2;
	buttonsHeight = Control::GetInstance()->ScreenHeight/12.0;

	titleFont = al_load_font(CalibriTTF, Control::GetInstance()->ScreenHeight/8.5, ALLEGRO_ALIGN_CENTER);

	// initializing buttons
	solvePuzzleButton = new Button("Solve Puzzle", buttonsX, solvePuzzleY, buttonsWidth, buttonsHeight);
	buttons.push_back(solvePuzzleButton);

	fullScreenButton = new Button("Fullscreen", buttonsX, fullScreenY, buttonsWidth, buttonsHeight);
	buttons.push_back(fullScreenButton);

	exitButton = new Button("Exit", buttonsX, exitY, buttonsWidth, buttonsHeight);
	buttons.push_back(exitButton);
}

bool MenuState::Update( ALLEGRO_EVENT *ev ) {
	// updating buttons
	for (Button *button: buttons)
		button->Update();

	// checking if any button was pressed
	if (solvePuzzleButton->wasReleased()) {
		Control::GetInstance()->ChangeState(_Solver);
		return true;
	}
	if (fullScreenButton->wasReleased()) {
		Control::GetInstance()->ToggleFullScreen();
		return true;
	}
	if (exitButton->wasReleased()) {
		Control::GetInstance()->setDoneState(true);
		return true;
	}

	return false;
}

void MenuState::Draw() {
	// printing title
	al_draw_text(titleFont, Black, Control::GetInstance()->ScreenWidth/2.0 + 3, titleY + 2, ALLEGRO_ALIGN_CENTER, "SUDOKU SOLVER");
	al_draw_text(titleFont, White, Control::GetInstance()->ScreenWidth/2.0, titleY, ALLEGRO_ALIGN_CENTER, "SUDOKU SOLVER");

	// drawing buttons
	for (Button *button: buttons)
		button->Draw();

	// printing author note
	al_draw_text(Control::GetInstance()->mediumFont, Black, Control::GetInstance()->ScreenWidth - Control::GetInstance()->mediumFont->height/2 + 1, Control::GetInstance()->ScreenHeight - Control::GetInstance()->mediumFont->height + 1, ALLEGRO_ALIGN_RIGHT, "Henrique Ferrolho - 2013");
	al_draw_text(Control::GetInstance()->mediumFont, White, Control::GetInstance()->ScreenWidth - Control::GetInstance()->mediumFont->height/2, Control::GetInstance()->ScreenHeight - Control::GetInstance()->mediumFont->height, ALLEGRO_ALIGN_RIGHT, "Henrique Ferrolho - 2013");
}

void MenuState::Terminate() {
	cout << "Deleting buttons..." << endl;
	// deleting buttons
	for (Button *button: buttons)
		delete button;
	buttons.clear();
}