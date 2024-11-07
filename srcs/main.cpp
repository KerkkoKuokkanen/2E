
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "image.h"
#include "Textures.h"
#include "renderSystem.h"
#include "commonTools.h"

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
	universalRenderingSystem.AddLayer(1, n_SortTypes::NO_SORT);
	Image *test2 = new Image(gameTestTextures.tile.text, {0.0, -0.5f, 0.8f, 1.0f}, 0.0f, 0);
	Image *test1 = new Image(gameTestTextures.hamis.text, {-0.6, -0.6f, 1.0f, 1.0f}, 0.0f, 0);
	test1->GetAccessToGLSprite()->SetColor(1.0f, 1.0f, 1.0f, 0.88f);
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		universalRenderingSystem.RenderAll();

		WindowSwap(window);
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
