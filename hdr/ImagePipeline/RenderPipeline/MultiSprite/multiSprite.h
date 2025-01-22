
#ifndef MULTI_SPRITE_H
# define MULTI_SPRITE_H

# include "IBO.h"

class MultiSprite
{
	private:
		float w = 1.0f;
		float h = 1.0f;
		IBO *ibo = NULL;
		std::vector<InstanceData> instances;
	public:
		MultiSprite(GLuint texture, float widht, float height, uint32_t maxSize);
		void Draw();
};

#endif
