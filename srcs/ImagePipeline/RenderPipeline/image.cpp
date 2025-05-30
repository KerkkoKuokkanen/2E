
#include "image.h"
#include "imageTransforms.h"
#include "commonTools.h"
#include "convexOverlap.h"
#include "Textures.h"

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

float Image::GetLowX()
{
	t_BoundingB data = sprite->GetBoundingB();
	float d_drawx = data.leftBottom.x;
	if (d_drawx > data.leftTop.x)
		d_drawx = data.leftTop.x;
	if (d_drawx > data.rightBottom.x)
		d_drawx = data.rightBottom.x;
	if (d_drawx > data.rightTop.x)
		d_drawx = data.rightTop.x;
	return (d_drawx);
}

Image::Image(std::string texture, t_Box rect, float angle, int layer)
{
	t_Point used1 = TransformCoordinateToScreenSpace(rect.x, rect.y);
	t_Point used2 = TransformCoordinateToScreenSpace(rect.w, rect.h);
	t_Texture data = GetTextureGLData(texture);
	Image::texture = data.text;
	sprite = new GLSprite({used1.x, used1.y}, {used2.x, used2.y}, Image::texture, defaultImageShader, 0);
	dimentions = {rect.w, rect.h};
	textIndex = data.hash;
	Image::angle = angle;
	sprite->SetRotation(angle);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	SetPosition(rect.x, rect.y);
}

Image::Image(uint64_t texture, t_Box rect, float angle, int layer)
{
	t_Point used1 = TransformCoordinateToScreenSpace(rect.x, rect.y);
	t_Point used2 = TransformCoordinateToScreenSpace(rect.w, rect.h);
	Image::texture = GetTextureGLSign(texture);
	sprite = new GLSprite({used1.x, used1.y}, {used2.x, used2.y}, Image::texture, defaultImageShader, 0);
	dimentions = {rect.w, rect.h};
	textIndex = texture;
	Image::angle = angle;
	sprite->SetRotation(angle);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	SetPosition(rect.x, rect.y);
}

void Image::SetColor(float r, float g, float b, float a)
{
	color.x = r;
	color.y = g;
	color.w = b;
	color.h = a;
	sprite->SetColor(r, g, b, a);
}

void Image::SetTexture(std::string name)
{
	t_Texture res = GetTextureGLData(name);
	textIndex = res.hash;
	texture = res.text;
	sprite->SetTexture(texture);
}

void Image::SetTexture(uint64_t hash)
{
	textIndex = hash;
	texture = GetTextureGLSign(hash);
	sprite->SetTexture(texture);
}

void Image::SetHeight(float height)
{
	if (transformType == n_TransformTypes::TRANSFORM_CAMERA)
		sprite->SetHeight(TransformHeightToCameraSpace(height));
	else
		sprite->SetHeight(height);
	ownHeight = height;
}

void Image::SetWidth(float width)
{
	if (transformType == n_TransformTypes::TRANSFORM_CAMERA)
		sprite->SetWidth(TransformWidthToCameraSpace(width));
	else
		sprite->SetWidth(width);
	ownWidth = width;
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
		default:
			used = TransformCoordinateToScreenSpace(x, y);
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
	if (!ReactangleScreenOverlap(data))
		return (true);
	return (false);
}

void Image::SetDrawY()
{
	drawY = GetLowY();
	drawX = GetLowX();
}

void Image::BeforeDraw()
{
	if (sprite == NULL)
		return ;
	SetPosition(position.x, position.y);
	SetWidth(ownWidth);
	SetHeight(ownHeight);
}

void Image::Draw()
{
	if (sprite == NULL)
		return ;
	if (!active)
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
	size_t dataSize = sizeof(float) * 11 + sizeof(uint64_t) + sizeof(int) * 2;
	return (dataSize);
}

void *Image::CollectSaveData(void *buffer, size_t buffSize, size_t &size)
{
	size_t dataSize = sizeof(float) * 11 + sizeof(uint64_t) + sizeof(int) * 2;
	size = dataSize;
	if (dataSize > buffSize)
		return (NULL);
	uint8_t *byteData = (uint8_t *)buffer;
	size_t offset = 0;
	memcpy(byteData + offset, &position.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &position.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &dimentions.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &dimentions.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &ownWidth, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &ownHeight, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &angle, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.w, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.h, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &textIndex, sizeof(uint64_t)); offset += sizeof(uint64_t);
	memcpy(byteData + offset, &transformType, sizeof(int)); offset += sizeof(int);
	memcpy(byteData + offset, &layer, sizeof(int));
	return (buffer);
}
