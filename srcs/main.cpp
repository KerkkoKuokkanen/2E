
#include "init.h"
#include "commonTools.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "screen.h"
#include "renderSystem.h"
#include "Textures.h"
#include "image.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

void MainLoop()
{
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	Image *test1 = new Image(gameTestTextures.hamis, {-0.5f, -0.4f, 1.0f, 1.0f}, 0.0f, 0);
	Image *test2 = new Image(gameTestTextures.hamis, {-0.5f, -0.7f, 1.0f, 1.0f}, 0.0f, 0);
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		//code here
		universalRenderingSystem.RenderAll();

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
