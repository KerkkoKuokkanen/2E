
#include "multiSprite.h"

MultiSprite::MultiSprite(GLuint texture, float widht, float height, uint32_t maxSize)
{
	InstanceData first = {{0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}};
	instances.reserve(maxSize);
	instances.push_back(first);
	ibo = new IBO(maxSize, texture);
	w = widht;
	h = height;
}

void MultiSprite::Draw()
{
	ibo->UpdateInstances(instances);
	ibo->Draw(w, h);
}
