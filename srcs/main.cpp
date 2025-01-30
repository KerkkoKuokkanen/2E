
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "renderSystem.h"
#include "commonTools.h"
#include "Textures.h"
#include "sysEnv.h"
#include "envHandler.h"

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
	//SysEnv *env = new SysEnv();
	//SystemObj *obj = new SystemObj();
	//Image *img = new Image(gameTestTextures.tile.text, {0.0f, 0.0f, 4.0f, 4.0f}, 0.0f, 0);
	//obj->AddComponent(img, n_ComponentTypes::IMAGE_CLASS);
	clock_t start, end;
	while(true)
	{
		//important
		start = clock();
		ClearWindow();
		Utility();

		//goof zone
		if (KeyPressed(SDL_SCANCODE_0))
			SaveEngineRoom();

		//important
		UpdateSysEnv();
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
	universalRenderingSystem.AddLayer(0, n_SortTypes::NO_SORT);
	LoadEngineRoom();
	MainLoop();
	return (0);
}
