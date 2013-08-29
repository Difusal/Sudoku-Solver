#pragma once

#include "stdIncludes.h"
#include "globalEnums.h"
#include "state.h"
#include "Mouse.h"

class Control {
public:
	static Control *GetInstance();
	void ChangeState(ControlState newState);

	void CreateAllegroDisplay(bool FullScreenMode);
	void DisplayLoadingSplashScreen();
	void StartMouseCursor();
	void LoadFonts();
	void CreateTimers();
	void CreateEventQueue();
	void LoadSoundSamples();
	void InitializeVariables();
	void StartTimers();

	void Initialize(bool FullScreenMode);
	bool StartControlCycle(bool FullScreenMode);
	void Update();
	void Draw();
	void Terminate();

	void ToggleFullScreen();

	ALLEGRO_DISPLAY *GetDisplay() { return display; }
	ALLEGRO_TIMER *GetTimer(TimerType Timer) { return timers[Timer]; }

	// Public Variables
	unsigned int ScreenWidth, ScreenHeight;

	MouseCursor *Mouse;

	vector<ALLEGRO_FONT*> fonts;
	ALLEGRO_FONT *largeFont;
	ALLEGRO_FONT *mediumFont;

private:
	static Control *instance;
	vector<State*> states;
	int state;

	vector<ALLEGRO_TIMER*> timers;
	ALLEGRO_TIMER *timer;
	
	bool togglingFullScreen;
	bool done, draw;

	ALLEGRO_DISPLAY *display;

	ALLEGRO_EVENT_QUEUE *eventQueue;
	ALLEGRO_EVENT ev;
};