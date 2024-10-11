
#include "LoadTexture.h"
#include "Textures.h"

t_TestTexts gameTestTextures;

void LoadTextures()
{
	gameTestTextures.hamis = LoadTexture("sprites/hämis1.png");
	gameTestTextures.tile = LoadTexture("sprites/Roof2_Color.png");
}
