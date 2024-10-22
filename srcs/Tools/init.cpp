
#include <OpenGL/gl3.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "ShaderClass.h"
#include "sprite.h"
#include "shape.h"
#include "Textures.h"
#include "lineDrawing.h"

#define WIDTH 1280
#define HEIGHT 720

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
												w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_SetWindowFullscreen(window, 0);
	//SDL_GL_SetSwapInterval(1); vsync
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, WIDTH, HEIGHT);

	//ALPHA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 

	return (window);
}

void InitSetup(Shader *shader)
{
	InitGLSprite(shader);
	InitShapes(shader);
	InitLines();
	LoadTextures();
}
