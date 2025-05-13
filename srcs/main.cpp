
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "envHandler.h"
#include "objBar.h"
#include "referenceComp.h"
#include "image.h"

SDL_Window *window = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

void MainLoop()
{
	/* SystemObj *obj1 = new SystemObj();
	Image *img = new Image("roof", {0.0f, 0.0f, 3.0f, 3.0f}, 0.0f, 0);
	Reference *ref = new Reference();
	obj1->AddComponent(img, n_ComponentTypes::IMAGE_CLASS);
	obj1->AddComponent(ref, "Reference");
	SystemObj *obj = new SystemObj();
	obj->AddComponent("ObjBar"); */
	clock_t start, end;
	while(true)
	{
		//important
		start = clock();
		ClearWindow();
		Utility();

		//goof zone

		//important
		UpdateSysEnv();
		WindowSwap(window);
		end = clock();
		SDL_Delay(figure_the_delay(start, end));
	}
}

int main()
{
	window = Init();
	Shader shader("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");
	InitSetup(&shader);
	universalRenderingSystem.Init();
	LoadEngineRoom();
	MainLoop();
	return (0);
}
