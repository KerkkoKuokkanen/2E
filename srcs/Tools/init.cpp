
#include "ShaderClass.h"
#include "sprite.h"
#include "shape.h"
#include "Textures.h"
#include "lineDrawing.h"
#include "mouse.h"
#include "screen.h"
#include "commonTools.h"
#include "image.h"
#include "structure.h"
#include "pillarBoxes.h"
#include "imageTransforms.h"

//2560 × 1600
#define WIDTH 1280
#define HEIGHT 800
#define FULL_SCREEN 0

int __currentScreenWidth = 0;
int __currentScreenHeight = 0;
unsigned int __currentScreenFrameRate = 0;
bool __forceAspectRatio = true;

SDL_Window *Init()
{
	srand((unsigned int)clock() + time(0));
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

	int w = WIDTH, h = HEIGHT;
	SDL_Window *window = SDL_CreateWindow("2E", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
												w, h, SDL_WINDOW_OPENGL);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_SetWindowFullscreen(window, 0);
	//SDL_GL_SetSwapInterval(1); vsync
	glDisable(GL_DEPTH_TEST);

	//ALPHA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	__currentScreenFrameRate = 60;
	SetFrameTime(rounding(1000.0f / (float)__currentScreenFrameRate));
	SDL_SetWindowFullscreen(window, FULL_SCREEN);
	SDL_GetWindowSize(window, &__currentScreenWidth, &__currentScreenHeight);
	glViewport(0, 0, __currentScreenWidth, __currentScreenHeight);
	//glClearColor(0.176f, 0.286f, 0.501f, 0.0f);
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	return (window);
}

void InitSetup(Shader *shader)
{
	InitShapes(shader);
	InitLines();
	InitImage(shader);
	InitStructure(shader);
	LoadTextures();
	SetPillarBoxes(shader);
	ResetImageTransformations();
	InitRenderSystem();
}
