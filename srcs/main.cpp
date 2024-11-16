
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
    void Init(void *add) override {

    }

    void Update() override {

    }

};

REGISTER_COMPONENT(PlayerMovement);

void MainLoop()
{
	universalRenderingSystem.AddLayer(0, n_SortTypes::Y_SORT);
	SystemObj *sysObj = new SystemObj;
	Image *test1 = new Image(gameTestTextures.hamis.text, {0.0f, 0.0f, 10.0f, 10.0f}, 0.0f, 0);
	unsigned int key = GetComponentKeyWithName("PlayerMovement");
	std::string name = GetComponentNameWithKey(key);
	sysObj->AddComponentCustom(name, NULL);
	sysObj->AddComponentStruct((void*)test1, n_ComponentTypes::IMAGE_CLASS, IMAGE_COMPONENT);
	sysObj->UpdateSystemObj();

	clock_t start, end;
	while(true)
	{
		start = clock();
		ClearWindow();
		Utility();

		sysObj->UpdateSystemObj();
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
