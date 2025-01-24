
#ifndef MULTI_SPRITE_H
# define MULTI_SPRITE_H

# include "IBO.h"

class MultiSprite
{
	private:
		float w = 1.0f;
		float h = 1.0f;
		IBO *ibo = NULL;
		uint32_t maxSize = 0;
		std::vector<uint32_t> freeKeys;
		std::vector<InstanceData> moddedData;
		std::vector<InstanceData> instances;
		void UpdateInstancesWithData();
	public:
		bool staticSprite = false;
		MultiSprite(GLuint texture, float widht, float height, uint32_t maxSize);
		~MultiSprite();
		uint32_t AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color);
		void RemoveSprite(uint32_t key);
		void ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color);
		void Draw();
};

#endif
