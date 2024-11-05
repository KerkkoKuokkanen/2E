
#include "init.h"
#include "commonTools.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "screen.h"
#include "renderSystem.h"
#include "Textures.h"
#include "image.h"
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
	0.0f, 0.5f,
	0.5f, 0.0f,
	0.0f, -0.5f,
	-0.5f, 0.0f
};

void MainLoop()
{
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	std::vector<float> vert;
	for (int i = 0; i < 8; i++)
		vert.push_back(verts[i]);
	t_DataForShape data = CreateGLShapeData(vert);
	Image *shape = new Image(gameTestTextures.hamis.text, {-0.5f, -0.5f, 1.0f, 1.0f}, 0.0f, 0);
	GLSprite *ass = shape->GetAccessToGLSprite();
	float xAdd = 0.0f, yAdd = 0.0f;
	float angle = 0.0f;
	float ww = 1.0f, hh = 1.0f;
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		//code here
		universalRenderingSystem.RenderAll();
		ass->SetPosition(xAdd, yAdd);
		ass->SetRotation(angle);
		ass->SetWidth(ww);
		ass->SetHeight(hh);
		if (KeyHeld(SDL_SCANCODE_UP))
			yAdd += 0.01f;
		if (KeyHeld(SDL_SCANCODE_DOWN))
			yAdd -= 0.01f;
		if (KeyHeld(SDL_SCANCODE_LEFT))
			xAdd -= 0.01f;
		if (KeyHeld(SDL_SCANCODE_RIGHT))
			xAdd += 0.01f;
		if (KeyHeld(SDL_SCANCODE_W))
			angle += 0.05f;
		if (KeyHeld(SDL_SCANCODE_S))
			angle -= 0.05f;
		if (KeyHeld(SDL_SCANCODE_0))
			ww -= 0.05f;
		if (KeyHeld(SDL_SCANCODE_1))
			ww += 0.05f;
		if (KeyHeld(SDL_SCANCODE_2))
			hh -= 0.05f;
		if (KeyHeld(SDL_SCANCODE_3))
			hh += 0.05f;

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
