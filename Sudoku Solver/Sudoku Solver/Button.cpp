#include "Button.h"
#include "Control.h"

Button::Button(string Label, unsigned int CenterX, unsigned int CenterY, unsigned int Width, unsigned int Height, unsigned int FontSize) {
	// default active value
	active = true;

	// setting button label
	label = Label;

	// setting button coordinates
	x = CenterX - Width/2.0;
	y = CenterY - Height/2.0;


	// setting button width and height
	width = Width;
	height = Height;


	// if font = 0, user did not specify font size, so use one according to the button height
	if (FontSize == 0)
		font = al_load_font(CalibriTTF, Height - 5, ALLEGRO_ALIGN_CENTER);
	else
		font = al_load_font(CalibriTTF, FontSize, ALLEGRO_ALIGN_CENTER);

	// checking if font was loaded correctly:
	if (!font) {
		al_show_native_message_box(Control::GetInstance()->GetDisplay(), "Error", "Could not load font file.", "Have you included the resources in the same directory of the program?", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}


	// initializing bool values
	beingHovered = false;
	beingPressed = false;
	released = false;
}

void Button::Draw() {
	// if button is not active, do nothing
	if (!active)
		return;

	// if button is being hovered use a different button color
	ALLEGRO_COLOR buttonColor;
	if (beingPressed)
		buttonColor = Gray;
	else if (beingHovered)
		buttonColor = LightGray;
	else if (!beingPressed && !beingHovered)
		buttonColor = White;

	// drawing button background
	al_draw_filled_rectangle(x, y, x+width, y+height, buttonColor);

	// drawing button contour
	al_draw_rectangle(x+3, y+3, x+width-3, y+height-3, DarkGray, 1.0);
	al_draw_rectangle(x+2, y+2, x+width-2, y+height-2, Gray, 1.0);
	al_draw_rectangle(x+1, y+1, x+width-1, y+height-1, LightGray, 1.0);
	al_draw_rectangle(x, y, x+width, y+height, Black, 1.0);

	// printing label
	al_draw_text(font, White, x + width/2.0 + 1, y + 0.1*font->height + 1, ALLEGRO_ALIGN_CENTER, label.c_str());
	al_draw_text(font, Black, x + width/2.0, y + 0.1*font->height, ALLEGRO_ALIGN_CENTER, label.c_str());
}

Button::~Button(void) {
	// nothing to do here ??
}