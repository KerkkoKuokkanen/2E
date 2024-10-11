
#include "init.h"
#include "sprite.h"
#include "Textures.h"
#include "commonTools.h"
#include "poller.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void MainLoop()
{
	GLSprite test(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), gameTestTextures.tile, shaderProgram);
	GLSprite test2(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), gameTestTextures.hamis, shaderProgram);

	clock_t start, end;

	while(true)
	{
		start = clock();
		Poller();
		ClearWindow();

		shaderProgram->Activate();
		test.Draw();
		test2.Draw();

		SDL_GL_SwapWindow(window);
		end = clock();
		SDL_Delay(figure_the_delay(start, end));
	}
}

int main()
{
	window = Init();
	Shader shader("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");
	InitGLSprite(shader);
	LoadTextures();
	shaderProgram = &shader;
	MainLoop();
	return (0);
}
