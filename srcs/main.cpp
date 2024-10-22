
#include "init.h"
#include "sprite.h"
#include "Textures.h"
#include "commonTools.h"
#include "poller.h"
#include <glm/gtc/type_ptr.hpp>
#include "shape.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

GLShape *GetTestShape()
{
	std::vector<float> points = {
		-0.5f, 0.5f, 
	    -0.2f, 0.0f,
	    -0.5f, -0.5f,
	    0.0f, -0.2f,
	    0.5f, -0.5f,
		0.2f, 0.0f,
		0.5f, 0.5f,
		0.0f, 0.2f
	};
	GLShape *ret = CreateGLShape(points, gameTestTextures.tile);
	return (ret);
}

void MainLoop()
{
	GLSprite	test(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), gameTestTextures.tile, shaderProgram);
	GLSprite	test2(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), gameTestTextures.hamis, shaderProgram);
	GLShape		*test3 = GetTestShape();

	clock_t start, end;
	while(true)
	{
		start = clock();
		Poller();
		ClearWindow();

		test3->Draw();

		SDL_GL_SwapWindow(window);
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
