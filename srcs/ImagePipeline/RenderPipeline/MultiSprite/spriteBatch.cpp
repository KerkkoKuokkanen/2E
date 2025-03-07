
#include "spriteBatch.h"
#include "Textures.h"

void SpriteBatch::SetUpMultiSprite(std::string texture, float widht, float height, uint32_t maxSize, int layer)
{
	if (multiSprite != NULL)
		delete multiSprite;
	t_Texture data = GetTextureGLData(texture);
	multiSprite = new MultiSprite(data.text, widht, height, maxSize, layer);
	SpriteBatch::width = widht;
	SpriteBatch::height = height;
	SpriteBatch::hash = data.hash;
	SpriteBatch::maxSize = maxSize;
	SpriteBatch::layer = layer;
}

void SpriteBatch::SetUpMultiSprite(uint64_t texture, float widht, float height, uint32_t maxSize, int layer)
{
	if (multiSprite != NULL)
		delete multiSprite;
	GLuint tex = GetTextureGLSign(texture);
	multiSprite = new MultiSprite(tex, widht, height, maxSize, layer);
	SpriteBatch::width = widht;
	SpriteBatch::height = height;
	SpriteBatch::hash = texture;
	SpriteBatch::maxSize = maxSize;
	SpriteBatch::layer = layer;
}

void SpriteBatch::SetSortingFunction(bool (*f)(InstanceData &one, InstanceData &two))
{
	if (multiSprite == NULL)
		return ;
	multiSprite->SortingFunction(f);
}

SpriteBatch::~SpriteBatch()
{
	if (multiSprite != NULL)
		delete multiSprite;
}

uint32_t SpriteBatch::AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (multiSprite == NULL)
		return (0);
	return (multiSprite->AddSprite(position, sRect, dimentions, angle, color));
}

void SpriteBatch::SetDepth(float depth)
{
	if (multiSprite == NULL)
		return ;
	multiSprite->SetDepth(depth);
}

void SpriteBatch::RemoveSprite(uint32_t key)
{
	if (multiSprite == NULL)
		return ;
	multiSprite->RemoveSprite(key);
}

void SpriteBatch::ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (multiSprite == NULL)
		return ;
	multiSprite->ModifySprite(key, position, sRect, dimentions, angle, color);
}
