
#include "../SDL2/SDL.h"
#include "../hdr/init.h"
#include "ShaderClass.h"
#include "sprite.h"
#include <glm/glm.hpp>
#include "LoadTexture.h"
#include <time.h>

#define FRAME 17

void Poller()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
	}
}

void ClearWindow()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

int	figure_the_delay(clock_t start, clock_t end)
{
	double	time;
	int		ret;
	time = 0.0;
	time += (double)(end - start) / CLOCKS_PER_SEC;
	ret = FRAME - (int)(time * 1000.0f);
	if (ret < 0)
		return (0);
	return (ret);
}

int main()
{
	SDL_Window *window = Init();
	Shader shaderProgram("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");
	InitGLSprite(shaderProgram);

	GLuint text = LoadTexture("Roof2_Color.png");
	GLuint text2 = LoadTexture("hämis1.png");
	GLSprite test(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), text, shaderProgram);
	GLSprite test2(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), text2, shaderProgram);

	clock_t start, end;

	while(true)
	{
		start = clock();
		Poller();
		ClearWindow();

		shaderProgram.Activate();
		test.Draw();
		test2.Draw();

		SDL_GL_SwapWindow(window);
		end = clock();
		SDL_Delay(figure_the_delay(start, end));
	}
	return (0);
}
