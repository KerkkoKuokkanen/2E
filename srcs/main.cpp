
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

void MainLoop()
{
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	SysEnv *env = new SysEnv();
	SystemObj *obj1 = new SystemObj(env);
	SystemObj *obj2 = new SystemObj(env);
	Image *img1 = new Image(gameTestTextures.hamis.text, {0.0f, 0.0f, 6.0f, 6.0f}, 0.0f, 0);
	Image *img2 = new Image(gameTestTextures.hamis.text, {-5.0f, -5.0f, 9.0f, 9.0f}, 22.0f, 0);
	obj1->AddComponent(img1, IMAGE_COMPONENT);
	obj2->AddComponent(img2, IMAGE_COMPONENT);
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		if (KeyPressed(SDL_SCANCODE_1))
		{
			int sign1 = (rand() % 2 == 0) ? (-1) : 1;
			int sign2 = (rand() % 2 == 0) ? (-1) : 1;
			int sign3 = (rand() % 2 == 0) ? (-1) : 1;
			int sign4 = (rand() % 2 == 0) ? (-1) : 1;
			img1->SetPosition(float_rand() * 5 * sign1, float_rand() * 5 * sign2);
			img2->SetPosition(float_rand() * 5 * sign3, float_rand() * 5 * sign4);
		}
		if (KeyPressed(SDL_SCANCODE_5))
			env->SaveState();
		if (KeyPressed(SDL_SCANCODE_6))
			env->LoadBack();
		if (KeyPressed(SDL_SCANCODE_0))
			delete obj1;
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
