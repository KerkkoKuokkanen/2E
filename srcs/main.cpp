
#include "init.h"
#include "commonTools.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "structure.h"
#include "Textures.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

float verts[] = {
	-0.2f, 0.0f,
	-0.5f, -0.5f,
	0.0f, -0.2f,
	0.5f, -0.5f,
	0.2f, 0.0f,
	0.5f, 0.5f,
	0.0f, 0.2f,
	-0.5f, 0.5f //eka
};

void MainLoop()
{
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	std::vector<float> points;
	for (int i = 0; i < 16; i++)
		points.push_back(verts[i]);
	t_DataForShape data = CreateGLShapeData(points);
	Structure *test = new Structure(data, gameTestTextures.tile.text, 0, true);
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
