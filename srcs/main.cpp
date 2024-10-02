
#include <stdio.h>
#include "../SDL2/SDL.h"
#include <OpenGL/gl3.h>
#include "../hdr/init.h"
#include "ShaderClass.h"
#include "sprite.h"
#include <glm/glm.hpp>

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

GLuint LoadTexture(const char* file) {
	SDL_Surface* surface = IMG_Load(file);
	SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	if (!surface)
	{
		printf("IMG_Load Error: %s\n", SDL_GetError());
		return (0);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return (textureID);
}

int main()
{
	SDL_Window *window = Init();
	Shader shaderProgram("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");

	GLuint text = LoadTexture("Roof2_Color.png");
	Sprite test(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), text, shaderProgram);
	Sprite test2(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), text, shaderProgram);

	while(true)
	{
		Poller();
		ClearWindow();

		shaderProgram.Activate();
		test.Draw();
		test2.DrawRect(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.5f, 0.5f));

		SDL_GL_SwapWindow(window);
	}
	return (0);
}
