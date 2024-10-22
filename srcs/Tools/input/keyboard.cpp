
#include "keyboard.h"

static bool currentKeyState[SDL_NUM_SCANCODES] = {0};
static bool previousKeyState[SDL_NUM_SCANCODES] = {0};

bool KeyPressed(int SDL_KeyCode)
{
	if (currentKeyState[SDL_KeyCode] && !previousKeyState[SDL_KeyCode])
		return (true);
	return (false);
}

bool KeyReleased(int SDL_KeyCode)
{
	if (currentKeyState[SDL_KeyCode] && !previousKeyState[SDL_KeyCode])
		return (true);
	return (false);
}

bool KeyHeld(int SDL_KeyCode)
{
	if (currentKeyState[SDL_KeyCode])
		return (true);
	return (false);
}

void UpdateKeyInputs()
{
	memcpy(previousKeyState, currentKeyState, sizeof(currentKeyState));
	const Uint8* state = SDL_GetKeyboardState(NULL);
	memcpy(currentKeyState, state, SDL_NUM_SCANCODES);

	// Check for a key that was just pressed
	if (currentKeyState[SDL_SCANCODE_W] && !previousKeyState[SDL_SCANCODE_W]) {
	    // The "W" key was just pressed
	}

	// Check for a key that was just released
	if (!currentKeyState[SDL_SCANCODE_W] && previousKeyState[SDL_SCANCODE_W]) {
	    // The "W" key was just released
	}

	// Check if a key is being held
	if (currentKeyState[SDL_SCANCODE_W]) {
	    // The "W" key is held down
}
}
