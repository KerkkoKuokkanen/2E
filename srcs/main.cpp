
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "image.h"
#include "Textures.h"
#include "renderSystem.h"
#include "textImage.h"

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
	universalRenderingSystem.AddLayer(1, n_SortTypes::TEXT_LAYER);
	Image *test1 = new Image(gameTestTextures.hamis.text, {-10.0f, -10.0f, 10.0f, 10.0f}, 0.0f, 0);
	TextImage *text = new TextImage("Haista kamu kettu, siunattu torttu!", {-5.0f, 5.0f}, 1);

	test1->SetPosition(0.0f, 0.0f);
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
