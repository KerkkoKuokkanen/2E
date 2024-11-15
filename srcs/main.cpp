
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "image.h"
#include "Textures.h"
#include "renderSystem.h"
#include "textImage.h"
#include "componentRegistry.h"
#include "systemObj.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

class PlayerMovement : public CustomComponent {
public:
    void Init(void * add) override {

    }

    void Update() override {
		printf("ade\n");
    }

};

REGISTER_COMPONENT(PlayerMovement);

void TestTest()
{
	SystemObj *sysObj = new SystemObj;
	sysObj->AddComponent("PlayerMovement");
	sysObj->UpdateSystemObj();
}

void MainLoop()
{
	TestTest();
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	universalRenderingSystem.AddLayer(1, n_SortTypes::TEXT_LAYER);
	Image *test1 = new Image(gameTestTextures.hamis.text, {-10.0f, -10.0f, 10.0f, 10.0f}, 0.0f, 0);
	TextImage *text = new TextImage("Haista kamu kettu, siunattu pottu!", {-5.0f, 5.0f}, 1);

	test1->SetPosition(0.0f, 0.0f);
	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

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
