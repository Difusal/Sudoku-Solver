#include "SolverState.h"
#include "Control.h"

void SolverState::Initialize() {
	board = new Board();

	unsigned int buttonX, buttonY;
	buttonX = board->X()+board->Width() + (Control::GetInstance()->ScreenWidth - (board->X()+board->Width())) / 2.0;
	buttonY = Control::GetInstance()->ScreenHeight / 2.0;
	submitButton = new Button("Submit", buttonX, buttonY, 130, 40);
	buttons.push_back(submitButton);

	puzzleHasBeenSubmitted = false;
}

bool SolverState::Update( ALLEGRO_EVENT *ev ) {
	// if event is a timer event
	if (ev->type == ALLEGRO_EVENT_TIMER) {
		// updating board
		board->Update();

		// updating buttons
		for (Button *button: buttons)
			button->Update();
	}

	switch (puzzleHasBeenSubmitted) {
		// if puzzle has been submitted so it can be solved
	case true:
		{
			break;
		}

		// if puzzle is still being edited
	case false:
		{
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
}

void SolverState::Terminate() {
}

void SolverState::DrawPuzzleBoard() {

}