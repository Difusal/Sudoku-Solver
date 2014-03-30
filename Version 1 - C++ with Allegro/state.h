#pragma once

#include "stdIncludes.h"

class State {
public:
	virtual void Initialize() = 0;
	virtual bool Update(ALLEGRO_EVENT *ev) = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
};