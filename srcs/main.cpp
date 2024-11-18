
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "Textures.h"
#include "renderSystem.h"
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
	-0.5f, 0.5f,
	0.5f, 0.5f,
	0.5f, -0.5f,
	-0.5f, -0.5f
};

void MainLoop()
{
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	std::vector<float> vertsVec;
	for (int i = 0; i < 8; i++)
		vertsVec.push_back(verts[i]);
	GLuint newShape = CreateGLShapeData(vertsVec);
	Structure *tester = new Structure(newShape, gameTestTextures.hamis.text, 0, true);
	Structure *tester2 = new Structure(newShape, gameTestTextures.tile.text, 0, true);
	float x = 0.0f, y = 0.0f;
	float w = 1.0f, h = 1.0f;
	float a = 0.0f;
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		if (KeyPressed(SDL_SCANCODE_LEFT))
			x -= 0.1f;
		if (KeyPressed(SDL_SCANCODE_RIGHT))
			x += 0.1f;
		if (KeyPressed(SDL_SCANCODE_UP))
			y += 0.1f;
		if (KeyPressed(SDL_SCANCODE_DOWN))
			y -= 0.1f;
		if (KeyPressed(SDL_SCANCODE_Q))
			w += 0.1f;
		if (KeyPressed(SDL_SCANCODE_A))
			w -= 0.1f;
		if (KeyPressed(SDL_SCANCODE_W))
			h += 0.1f;
		if (KeyPressed(SDL_SCANCODE_S))
			h -= 0.1f;
		if (KeyPressed(SDL_SCANCODE_E))
			a += 0.1f;
		if (KeyPressed(SDL_SCANCODE_D))
			a -= 0.1f;
		tester->SetTextureData(0.0f, 0.0f, w, h, a);
		tester->SetPosition(x, y);
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
