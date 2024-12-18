
#ifndef MOUSE_H
# define MOUSE_H

# include <SDL2/SDL.h>
# include "box.h"
#include <tuple>

int GetScrollWheelValue();
void UpdateMouse();
t_Point GetMouseXY();
bool WheelIn();
bool WheelOut();
bool MouseKeyPressed(int mouseKey);
bool MouseKeyHeld(int mouseKey);
bool MouseKeyReleased(int mouseKey);
std::tuple<int, int> GetRawMouseXY();

namespace n_MouseKeys {
	enum{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_4,
		MOUSE_5
	};
};

#endif
