
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
			AddToSave(&i, sizeof(int) * 600);
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
	Image *img1 = new Image(gameTestTextures.everyColor.text, {-5.0f, -5.0f, 3.0f, 3.0f}, 0.0f, 0);
	Image *img2 = new Image(gameTestTextures.everyColor.text, {-3.0f, -3.0f, 3.0f, 3.0f}, 0.0f, 0);
	obj1->AddComponent(img1, IMAGE_COMPONENT);
	obj1->AddComponent(img2, IMAGE_COMPONENT);
	uint64_t objKey = obj1->SystemObjectKey();;
	float x = 0.0f, y = 0.0f;
	float w = 0.0f, h = 0.0f;
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
			w -= 0.2f;
		if (KeyHeld(SDL_SCANCODE_Q))
			w += 0.2f;
		if (KeyHeld(SDL_SCANCODE_S))
			h -= 0.2f;
		if (KeyHeld(SDL_SCANCODE_W))
			h += 0.2f;
		if (KeyHeld(SDL_SCANCODE_E))
			angle += 0.05f;
		if (KeyHeld(SDL_SCANCODE_D))
			angle -= 0.05f;
		if (KeyPressed(SDL_SCANCODE_5))
			env->SaveState();
		if (KeyPressed(SDL_SCANCODE_6))
			env->LoadBack(SNAPSHOT_PREVIOUS);

		SystemObj *used = env->FindObject(objKey);
		used->transform->Angle(angle);
		used->transform->Position(x, y);


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
