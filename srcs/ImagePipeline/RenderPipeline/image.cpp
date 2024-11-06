
#include "image.h"
Shader *defaultImageShader;

float Image::GetLowY()
{
	t_BoundingB data = sprite->GetBoundingB();
	float d_drawY = data.leftBottom.y;
	if (d_drawY > data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY > data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY > data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

Image::Image(GLuint texture, t_Box rect, float angle, int layer)
{
	sprite = new GLSprite({rect.x, rect.y}, {rect.w, rect.h}, texture, defaultImageShader, 0);
	drawY = GetLowY();
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
