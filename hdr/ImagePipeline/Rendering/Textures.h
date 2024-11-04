
#ifndef TEXTURES_H
# define TEXTURES_H

# include <OpenGL/gl3.h>

typedef struct s_TestTexts
{
	GLuint tile;
	GLuint hamis;
}				t_TestTexts;

extern t_TestTexts gameTestTextures;

void LoadTextures();

#endif
