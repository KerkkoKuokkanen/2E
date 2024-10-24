
#include "init.h"
#include "commonTools.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "screen.h"
#include "shape.h"
#include "Textures.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

float vertData[] = {
	-0.5f, 0.5f,
	0.0f, 0.2f,
	0.5f, 0.5f,
	0.2f, 0.0f,
	0.5f, -0.5f,
	0.0f, -0.2f,
	-0.5f, -0.5f,
	-0.2f, 0.0f
};

void MainLoop()
{
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		//code here

		SDL_GL_SwapWindow(window);
		end = clock();
		SDL_Delay(figure_the_delay(start, end));
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
