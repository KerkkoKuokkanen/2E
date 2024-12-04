
#include "image.h"
#include "imageTransforms.h"
#include "commonTools.h"
#include "convexOverlap.h"

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
	dimentions = {rect.w, rect.h};
	Image::texture = texture;
	Image::angle = angle;
	sprite->SetRotation(angle);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	SetPosition(rect.x, rect.y);
}

void Image::SetPosition(float x, float y)
{
	if (sprite == NULL)
		return ;
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
	position = {x, y};
}

bool Image::OffscreenDetection()
{
	if (sprite->shape == NULL)
		return (true);
	t_BoundingB data = sprite->GetBoundingB();
	if (!ReactangleScreenOverlap(data) && sprite->shape->detectOverScreenOff == false)
		return (true);
	return (false);
}

void Image::Draw()
{
	if (sprite == NULL)
		return ;
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

size_t Image::GetSaveDataSize()
{
	size_t dataSize = sizeof(float) * 5 + sizeof(GLuint) + sizeof(int);
	return (dataSize);
}

void *Image::CollectSaveData(void *buffer, size_t buffSize, size_t &size)
{
	size_t dataSize = sizeof(float) * 5 + sizeof(GLuint) + sizeof(int);
	size = dataSize;
	if (dataSize > buffSize)
		return (NULL);
	uint8_t *byteData = (uint8_t *)buffer;
	size_t offset = 0;
	memcpy(byteData + offset, &position.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &position.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &dimentions.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &dimentions.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &angle, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &texture, sizeof(GLuint)); offset += sizeof(GLuint);
	memcpy(byteData + offset, &layer, sizeof(int));
	return (buffer);
}
