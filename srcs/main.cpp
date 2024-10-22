
#include "init.h"
#include "commonTools.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "screen.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

const int fixedUpdateFPS = 60;
const double fixedTimeStep = 1000.0 / fixedUpdateFPS;

Uint32 previousTime = 0;
double accumulator = 0.0;

static void Polling()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

static void FixedUpdate()			// 60 tics / sec
{

}

static void Update()				// custom refresh rate
{
	ClearWindow();
	SDL_GL_SwapWindow(window);
}

void MainLoop()
{
	previousTime = SDL_GetTicks();
	Uint32 targetFPS = __currentScreenFrameRate; //framerate of the game
	Uint32 renderFrameTime = 1000 / targetFPS;
	Uint32 lastRenderTime = 0; 
	while(true)
	{
		Polling();
		Uint32 currentTime = SDL_GetTicks();
		double elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		accumulator += elapsedTime;
		while (accumulator >= fixedTimeStep)
		{
			FixedUpdate();
			accumulator -= fixedTimeStep;
		}
		if (currentTime - lastRenderTime >= renderFrameTime)
		{
			Update();
			lastRenderTime = currentTime;
		}
		SDL_Delay(1);
	}
}

int main()
{
	window = Init();
	Shader shader("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");
	shaderProgram = &shader;
	InitSetup(shaderProgram);
	MainLoop();
	return (0);
}
