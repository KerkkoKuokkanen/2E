
#include "image.h"
Shader *defaultImageShader;


Image::Image(GLuint texture, t_Box rect, float angle, int layer)
{
	sprite = new GLSprite({rect.x, rect.y}, {rect.w, rect.h}, texture, defaultImageShader, 0);
	drawY = rect.y;
	AddToRenderSystem(layer);
}

void Image::Draw()
{
	sprite->Draw();
}

Image::~Image()
{
	if (sprite != NULL)
		delete (sprite);
}

void InitImage(Shader *usedShader)
{
	defaultImageShader = usedShader;
}
