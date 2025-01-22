
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "renderSystem.h"
#include "commonTools.h"
#include "sysSaver.h"
#include "customComponent.h"
#include "componentRegistry.h"
#include "sysEnv.h"
#include "image.h"
#include "Textures.h"
#include "structure.h"
#include "imageTransforms.h"
#include "shapeSaving.h"
#include "multiSprite.h"

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
	universalRenderingSystem.AddLayer(0, n_SortTypes::DEPTH_SORT);
	universalRenderingSystem.AddLayer(LINE_LAYER, n_SortTypes::NO_SORT);
	MultiSprite *mul = new MultiSprite(gameTestTextures.colorTester.text, 1.0f, 1.0f, 100);
	uint64_t data = LoadShape("saves/shapes/tester.shape");
	SysEnv *env = new SysEnv();
	clock_t start, end;
	while(true)
	{
		//important
		start = clock();
		ClearWindow();
		Utility();

		//goof zone
		mul->Draw();

		//important
		env->UpdateSysObjects();
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
