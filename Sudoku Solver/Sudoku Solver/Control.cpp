#include "Control.h"

#include "MenuState.h"
#include "SolverState.h"

Control *Control::instance = NULL;

Control *Control::GetInstance() {
	if (!instance)
		instance = new Control();

	return instance;
}

void Control::ChangeState(ControlState newState) {
	// if there was no state before, do not terminate a state which was not even initialized
	if (state != -1)
		states[state]->Terminate();

	state = newState;
	states[state]->Initialize();
}

void Control::CreateAllegroDisplay(bool FullScreenMode) {
	cout << "Creating display..." << endl;

	switch (FullScreenMode) {
	case false:
		// creating a window
		{
			// specifying program to run on a window
			al_set_new_display_flags(ALLEGRO_WINDOWED);

			// default values
			ScreenWidth = DefaultScreenWidth;
			ScreenHeight = DefaultScreenHeight;

			break;
		}
	case true:
		// creating full screen display
		{
			// specifying program to run on full screen
			al_set_new_display_flags(ALLEGRO_FULLSCREEN);

			// getting monitor info
			ALLEGRO_DISPLAY_MODE disp_data;
			al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);

			// setting screen width and height
			ScreenWidth = disp_data.width;
			ScreenHeight = disp_data.height;

			break;
		}
	}

	// creating display
	display = al_create_display(ScreenWidth, ScreenHeight);

	// if display was not loaded correctly, show error message and quit program
	if (!display) {
		al_show_native_message_box(display, "Error", "Display Settings", "Couldn't create a display.\nQuitting program.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	// changing window title
	al_set_window_title(display, ProgramTitle);
}

void Control::LoadFonts() {
	cout << "Loading fonts..." << endl;

	largeFont = al_load_font(CalibriTTF, 70, ALLEGRO_ALIGN_CENTER);
	fonts.push_back(largeFont);

	mediumFont = al_load_font(CalibriTTF, 20, ALLEGRO_ALIGN_CENTER);
	fonts.push_back(mediumFont);


	// checking if every font was loaded correctly
	for (unsigned int i = 0; i < fonts.size(); i++) {
		if (!fonts[i]) {
			al_show_native_message_box(display, "Error", "Could not load font file.", "If you have just created the project, create the following folder structure:\n\"yourProjectFolder\\res\\fonts\"\n\nThen place calibri.ttf inside the fonts folder.\nYou can get the font on the internet easily.\n\nPress OK to quit the program.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
		}
	}
}

void Control::DisplayLoadingSplashScreen() {
	cout << "Displaying loading game background..." << endl;
	
	// setting background color
	al_clear_to_color(DarkGray);

	// printing text
	al_draw_text(largeFont, Black, ScreenWidth/2.0 + 1, ScreenHeight/2.0 + 2, ALLEGRO_ALIGN_CENTER, "Loading...");
	al_draw_text(largeFont, White, ScreenWidth/2.0, ScreenHeight/2.0, ALLEGRO_ALIGN_CENTER, "Loading...");

	// flipping display
	al_flip_display();
}

void Control::StartMouseCursor() {
	cout << "Starting mouse cursor..." << endl;
	Mouse = new MouseCursor();
}

void Control::CreateTimers() {
	cout << "Creating timers..." << endl;

	timer = al_create_timer(1.0 / FPS);
	timers.push_back(timer);
}

void Control::CreateEventQueue() {
	cout << "Creating event queues..." << endl;
	eventQueue = al_create_event_queue();

	cout << "Registering event sources..." << endl;
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_mouse_event_source());
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	for (unsigned int i = 0; i < timers.size(); i++)
		al_register_event_source(eventQueue, al_get_timer_event_source(timers[i]));
}

void Control::LoadSoundSamples() {
	cout << "Stating how many sounds can play simultaneously..." << endl;
	al_reserve_samples(3);

	cout << "Loading audio samples..." << endl;
	/*
	lineClearSound = al_load_sample(LineClearSound);
	explosionSound = al_load_sample(ExplosionSound);
	themeSong = al_load_sample(ThemeSong);
	if (!themeSong) {
	al_show_native_message_box(Tetris::GetInstance()->GetDisplay(), "Error", "Could not load Tetris theme song.", "Your resources folder must be corrupt, please download it again.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	exit(-1);
	}*/
}

void Control::InitializeVariables() {
	cout << "Initializing variables..." << endl;
	togglingFullScreen = false;
	done = false;
	draw = true;
}

void Control::StartTimers() {
	cout << "Starting timers..." << endl;
	for (unsigned int i = 0; i < timers.size(); i++)
		al_start_timer(timers[i]);
}


void Control::Initialize(bool FullScreenMode) {
	cout << endl;
	cout << "###########################" << endl;
	cout << "##                       ##" << endl;
	cout << "##    STARTING EDITOR    ##" << endl;
	cout << "##                       ##" << endl;
	cout << "###########################" << endl;
	cout << endl;
	cout << "-------------" << endl;
	cout << "Activity Log:" << endl;
	cout << "-------------" << endl;

	cout << "Getting time seed for random numbers..." << endl;
	srand ((unsigned int) time(NULL));

	CreateAllegroDisplay(FullScreenMode);
	LoadFonts();
	DisplayLoadingSplashScreen();
	StartMouseCursor();
	CreateTimers();
	CreateEventQueue();
	LoadSoundSamples();
	InitializeVariables();
	StartTimers();
}

bool Control::StartControlCycle(bool FullScreenMode) {
	Initialize(FullScreenMode);

	states.push_back(new MenuState());
	states.push_back(new SolverState());
	state = -1;
	ChangeState(_Menu);

	cout << "Starting game control cycle..." << endl;
	while (!done) {
		al_wait_for_event(eventQueue, &ev);

		Update();

		Draw();
	}
	states[state]->Terminate();

	bool returnValue = togglingFullScreen;

	Terminate();

	return returnValue;
}

void Control::Update() {
	// if window is closed on dedicated button (upper right corner)
	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		cout << "Close button pressed..." << endl;
		done = true;
	}

	// updating mouse
	draw = Mouse->Update(&ev);

	// correcting mouse position due to camera movement
	if (ev.type != ALLEGRO_EVENT_MOUSE_AXES)
		draw = Mouse->CorrectMousePosition();

	// updating current state
	states[state]->Update(&ev);

	// setting all mouse release values to false on timer event
	if (ev.type == ALLEGRO_EVENT_TIMER) {
		Mouse->SetAllReleaseValuesToFalse();
		draw = true;
	}
}

void Control::Draw() {
	if (draw && al_event_queue_is_empty(eventQueue)) {
		// drawing current state
		states[state]->Draw();

		// ---------------
		// Debugging code:
		// Uncomment this block of code to display mouse coords.
		// -----------------------------------------------------
		/*
		stringstream ss;
		ss << "x:" << Mouse->x << " y:" << Mouse->y;
		al_draw_text(mediumFont, Yellow, 0, 0, NULL, ss.str().c_str());
		// cout << ss.str() << endl;
		*/

		// flipping display and preparing buffer for next cycle
		al_flip_display();
		al_clear_to_color(LightBlue);
		draw = false;
	}
}

void Control::Terminate() {
	cout << "Deallocating memory and quitting..." << endl;

	// audio samples


	// deleting mouse cursor
	delete Mouse;

	// destroying display
	al_destroy_display(display);

	// destroying fonts
	for (unsigned int i = 0; i < fonts.size(); i++)
		al_destroy_font(fonts[i]);
	fonts.clear();

	// destroying event queue
	al_destroy_event_queue(eventQueue);

	// destroying timers
	for (unsigned int i = 0; i < timers.size(); i++)
		al_destroy_timer(timers[i]);
	timers.clear();
}


void Control::ToggleFullScreen() {
	cout << "Toggling full screen..." << endl;
	togglingFullScreen = true;
	done = true;
}