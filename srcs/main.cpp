
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
	Image *img2 = new Image(gameTestTextures.everyColor.text, {3.0f, 3.0f, 5.0f, 5.0f}, 0.0f, 0);
	Image *img3 = new Image(gameTestTextures.everyColor.text, {-3.0f, -3.0f, 5.0f, 5.0f}, 0.0f, 0);
	img2->SetAngle(1.0f);
	obj1->AddComponent(str, STRUCTURE_COMPONENT);
	obj1->AddComponent(img2, IMAGE_COMPONENT);
	obj1->AddComponent(img3, IMAGE_COMPONENT);
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

		if (KeyHeld(SDL_SCANCODE_LEFT))
			x -= 0.2f;
		if (KeyHeld(SDL_SCANCODE_RIGHT))
			x += 0.2f;
		if (KeyHeld(SDL_SCANCODE_UP))
			y += 0.2f;
		if (KeyHeld(SDL_SCANCODE_DOWN))
			y -= 0.2f;
		if (KeyHeld(SDL_SCANCODE_A))
			w -= 0.1f;
		if (KeyHeld(SDL_SCANCODE_Q))
			w += 0.1f;
		if (KeyHeld(SDL_SCANCODE_S))
			h -= 0.1f;
		if (KeyHeld(SDL_SCANCODE_W))
			h += 0.1f;
		if (KeyHeld(SDL_SCANCODE_E))
			angle += 0.05f;
		if (KeyHeld(SDL_SCANCODE_D))
			angle -= 0.05f;
		if (KeyPressed(SDL_SCANCODE_5))
			angle = 0.0f;
		if (KeyPressed(SDL_SCANCODE_P))
			env->SaveState();
		if (KeyPressed(SDL_SCANCODE_O))
			env->LoadBack(-1);

		SystemObj *used = env->FindObject(objKey);
		used->transform->Position(x, y);
		used->transform->Angle(angle);
		used->transform->Width(w);
		used->transform->Height(h);


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
