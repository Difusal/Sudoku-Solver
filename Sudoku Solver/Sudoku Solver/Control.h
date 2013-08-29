#pragma once

#include "stdIncludes.h"
#include "globalEnums.h"
#include "state.h"
#include "Mouse.h"

class Control {
public:
	static Control *GetInstance();
	void ChangeState(ControlState newState);

	void StartAllegro5();
	void CreateAllegroDisplay();
	void DisplayLoadingSplashScreen();
	void StartMouseCursor();
	void LoadFonts();
	void CreateTimers();
	void CreateEventQueue();
	void LoadSoundSamples();
	void InitializeVariables();
	void StartTimers();

	void StartControlCycle();
	void Initialize();
	void Update();
	void Draw();
	void Terminate();

	ALLEGRO_DISPLAY *GetDisplay() { return display; }
	ALLEGRO_TIMER *GetTimer(TimerType Timer);


	// Public Variables
	MouseCursor *Mouse;
	float cameraPosition[2];

	vector<ALLEGRO_FONT*> fonts;
	ALLEGRO_FONT *largeFont;
	ALLEGRO_FONT *mediumFont;

private:
	static Control *instance;
	vector<State*> states;
	int state;

	vector<ALLEGRO_TIMER*> timers;
	ALLEGRO_TIMER *timer;
		
	bool done, draw;

	ALLEGRO_DISPLAY *display;

	ALLEGRO_EVENT_QUEUE *eventQueue;
	ALLEGRO_EVENT ev;
};