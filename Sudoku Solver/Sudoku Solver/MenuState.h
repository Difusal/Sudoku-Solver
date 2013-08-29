#pragma once

#include "stdIncludes.h"
#include "state.h"

class MenuState: public State {
public:
	virtual void Initialize();
	virtual bool Update( ALLEGRO_EVENT *ev );
	virtual void Draw();
	virtual void Terminate();
};