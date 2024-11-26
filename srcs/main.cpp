
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

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

class PMove : public CustomComponent
{
	public:
		void Init(void *data, size_t size) override {
			int i = 88;
			AddToSaveData(&i, sizeof(int) * 600);
		};
		void Update() override {
			printf("moimoi");
		};
};

REGISTER_COMPONENT(PMove);

void SysSaverTest()
{
	SystemSaver *test = new SystemSaver();
	SystemObj *used1 = new SystemObj(NULL);
	used1->saveable = true;
	used1->AddComponentCustom("PMove", NULL, 0);
	for (int i = 0; i < 1500; i++)
	{
		SystemObj *used1 = new SystemObj(NULL);
		used1->saveable = true;
		used1->AddComponentCustom("PMove", NULL, 0);
		test->SaveSystemObj(used1);
	}
	test->TakeSnapShot();
}

void MainLoop()
{
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		SysSaverTest();
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
