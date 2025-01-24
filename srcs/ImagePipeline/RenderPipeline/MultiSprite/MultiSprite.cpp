
#include "multiSprite.h"
#include "imageTransforms.h"

MultiSprite::~MultiSprite()
{
	delete ibo;
}

MultiSprite::MultiSprite(GLuint texture, float widht, float height, uint32_t maxSize)
{
	t_Point wh = TransformCoordinateToScreenSpace(widht, height);
	MultiSprite::maxSize = maxSize;
	instances.reserve(maxSize);
	moddedData.reserve(maxSize);
	freeKeys.reserve(maxSize);
	for (int i = 0; i < maxSize; i++)
	{
		freeKeys.push_back(i + 1);
		moddedData.push_back({0});
		moddedData[i].key = 0;
	}
	ibo = new IBO(maxSize, texture);
	w = wh.x;
	h = wh.y;
}

uint32_t MultiSprite::AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (instances.size() >= maxSize)
		return (0);
	if (staticSprite)
		position = TransformCoordinateToScreenSpace(position.x, position.y);
	else
		position = TransformCoordinateToScreenSpaceCamera(position.x, position.y);
	uint32_t key = freeKeys[freeKeys.size() - 1];
	freeKeys.pop_back();
	InstanceData first = {position, sRect, dimentions, angle, color, key};
	instances.push_back(first);
	return (key);
}

void MultiSprite::RemoveSprite(uint32_t key)
{
	moddedData[key - 1].key = 2;
}

void MultiSprite::ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (staticSprite)
		position = TransformCoordinateToScreenSpace(position.x, position.y);
	else
		position = TransformCoordinateToScreenSpaceCamera(position.x, position.y);
	moddedData[key - 1].key = 1;
	moddedData[key - 1].angle = angle;
	moddedData[key - 1].position = position;
	moddedData[key - 1].scale = dimentions;
	moddedData[key - 1].texUV = sRect;
	moddedData[key - 1].color = color;
}

void MultiSprite::UpdateInstancesWithData()
{
	for (int i = 0; i < instances.size(); i++)
	{
		uint32_t key = instances[i].key;
		if (moddedData[key - 1].key == 0)
			continue ;
		if (moddedData[key - 1].key == 2)
		{
			freeKeys.push_back(instances[i].key);
			instances[i].key = 0;
			continue ;
		}
		instances[i] = moddedData[key - 1];
		instances[i].key = key;
	}
	instances.erase(
	std::remove_if(instances.begin(), instances.end(),
				[](const InstanceData& instance) {
					return instance.key == 0;
				}),
	instances.end());
}

void MultiSprite::Draw()
{
	UpdateInstancesWithData();
	ibo->UpdateInstances(instances);
	ibo->Draw(w, h);
}
