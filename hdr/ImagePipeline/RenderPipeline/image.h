
#ifndef IMAGE_H
# define IMAGE_H

# include "sprite.h"
# include "renderSystem.h"

class Image : public RenderObj
{
	private:
		GLSprite *sprite = NULL;
		float GetLowY();
		GLuint texture = 0;
		t_Point position = {0.0f, 0.0f};
		t_Point dimentions = {0.0f, 0.0f};
		float angle = 0.0f;
		int transformType = n_TransformTypes::TRANSFORM_CAMERA;
	public:
		Image(GLuint texture, t_Box rect, float angle, int layer);
		~Image();
		void *CollectSaveData(void *buffer, size_t buffSize, size_t &size);
		GLSprite *GetAccessToGLSprite() {return (sprite);};
		void SetTransformType(int tType) {transformType = tType;};
		void SetPosition(float x, float y);
		void Draw() override;

};

void InitImage(Shader *usedShader);

#endif
