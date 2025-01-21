
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
	uint64_t data = LoadShape("saves/shapes/tester.shape");
	SysEnv *env = new SysEnv();
	SystemObj *obj1 = new SystemObj(env);
	Structure *str = new Structure(data, gameTestTextures.everyColor.text, 0, false);
	Image *img = new Image(gameTestTextures.hamis.text, {-3.0f, -3.0f, 3.0f, 3.0f}, 0.0f, 0);
	obj1->AddComponent(str, STRUCTURE_COMPONENT);
	obj1->AddComponent(img, IMAGE_COMPONENT);
	uint64_t objKey = obj1->SystemObjectKey();;
	float x = 0.0f, y = 0.0f;
	float w = 1.0f, h = 1.0f;
	float angle = 0.0f;
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		if (KeyPressed(SDL_SCANCODE_0))
		{
			SystemObj *used = env->FindObject(objKey);
			used->transform->Angle(float_rand());
			used->transform->Position(float_rand(), float_rand());
			used->transform->Width(float_rand());
			used->transform->Height(float_rand());
		}
		if (KeyPressed(SDL_SCANCODE_P))
			env->SaveState();
		if (KeyPressed(SDL_SCANCODE_O))
			env->LoadBack(-1);

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
