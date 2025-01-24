
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
	MultiSprite *mul = new MultiSprite(gameTestTextures.colorTester.text, 1.0f, 1.0f, 120000);
	std::vector<uint32_t> keys;
	for (int i = 0; i < 119000; i++)
		keys.push_back(mul->AddSprite({0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}));
	SysEnv *env = new SysEnv();
	clock_t start, end;
	float pos1 = 0.0f, pos2 = 0.0f;
	while(true)
	{
		//important
		start = clock();
		ClearWindow();
		Utility();

		//goof zone
		for (int i = 0; i < keys.size(); i++)
		{
			mul->ModifySprite(keys[i], {float_rand() * 10.0f * (float)pow(-1, rand() % 2), float_rand() * 10.0f * (float)pow(-1, rand() % 2)}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f});
			if (rand() % 90 == 0)
				mul->RemoveSprite(keys[i]);
		}
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
