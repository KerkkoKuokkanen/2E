
#ifndef TEXTURES_H
# define TEXTURES_H

# include <OpenGL/gl3.h>
# include "../frameworks/SDL2/SDL.h"

typedef struct s_Texture
{
	GLuint text;
	SDL_Surface *sur;
}				t_Texture;

typedef struct s_TestTexts
{
	t_Texture tile;
	t_Texture hamis;
	t_Texture everyColor;
	t_Texture colorTester;
	t_Texture testAtlas;
	t_Texture selectedObj;
}				t_TestTexts;

extern t_TestTexts gameTestTextures;

void LoadTextures();

#endif
