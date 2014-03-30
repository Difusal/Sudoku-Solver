#pragma once

#include "BasicButton.h"

class Button: public BasicButton {
public:
	Button(string Label, unsigned int CenterX, unsigned int CenterY, unsigned int Width, unsigned int Height, unsigned int FontSize = 0);
	void Draw();
	~Button(void);
};