
#include "image.h"
#include "imageTransforms.h"
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
	t_Point used1 = TransformCoordinateToScreenSpace(rect.x, rect.y);
	t_Point used2 = TransformCoordinateToScreenSpace(rect.w, rect.h);
	sprite = new GLSprite({used1.x, used1.y}, {used2.x, used2.y}, texture, defaultImageShader, 0);
	position = {rect.x, rect.y};
	drawY = GetLowY();
	AddToRenderSystem(layer);
}

void Image::SetPosition(float x, float y)
{
	position = {x, y};
}

void Image::Draw()
{
	if (sprite == NULL)
		return ;
	float x = position.x;
	float y = position.y;
	t_Point used;
	switch (transformType)
	{
		case n_TransformTypes::TRANSFORM_CAMERA:
			used = TransformCoordinateToScreenSpaceCamera(x, y);
			break ;
		case n_TransformTypes::TRANSFORM_STATIC:
			used = TransformCoordinateToScreenSpace(x, y);
		default:
			used = {x, y};
			break ;
	}
	sprite->SetPosition(used.x, used.y);
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
