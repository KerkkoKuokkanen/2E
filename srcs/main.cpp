
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "image.h"
#include "Textures.h"
#include "renderSystem.h"
#include "commonTools.h"
#include "structure.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

float verts[] = {
	-5.0f, 5.0f,
	0.0f, 2.0f,
	5.0f, 5.0f,
	2.0f, 0.0f,
	5.0f, -5.0f,
	0.0f, -2.0f,
	-5.0f, -5.0f,
	-2.0f, 0.0f
};

void MainLoop()
{
	std::vector<float> points;
	for (int i = 0; i < 16; i++)
		points.push_back(verts[i] * 0.1f);
	t_DataForShape data = CreateGLShapeData(points);
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	universalRenderingSystem.AddLayer(1, n_SortTypes::NO_SORT);
	Image *test1 = new Image(gameTestTextures.hamis.text, {-10.0f, -10.0f, 10.0f, 10.0f}, 0.0f, 1);
	Structure *test2 = new Structure(data, gameTestTextures.tile.text, 0, false);
	test1->SetPosition(0.0f, 0.0f);
	test2->SetPosition(-5.0f, 0.0f);
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
