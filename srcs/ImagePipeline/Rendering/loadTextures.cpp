
#include "LoadTexture.h"
#include "Textures.h"

t_TestTexts gameTestTextures;

void LoadTextures()
{
	gameTestTextures.hamis = LoadTexture("sprites/defaults/hämis1.png");
	gameTestTextures.tile = LoadTexture("sprites/defaults/Roof2_Color.png");
	gameTestTextures.everyColor = LoadTexture("sprites/defaults/everyColor.png");
}
