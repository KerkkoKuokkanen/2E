
#ifndef STRUCTURE_H
# define STRUCTURE_H

# include "shapeEX.h"
# include "renderSystem.h"

class Structure : public RenderObj
{
	private:
		GLShape *shape = NULL;
		float GetLowY();
		bool textModdingEnabled = false;
		float angle = 0.0f;
		t_Point position = {0.0f, 0.0f};
		GLuint texture = 0, shapeData = 0;
		int transformType = n_TransformTypes::TRANSFORM_CAMERA;
	public:
		bool saveable = false;
		Structure(GLuint shape, GLuint texture, int layer, bool textModding);
		~Structure();
		size_t GetSaveDataSize();
		void *CollectSaveData(void *buffer, size_t buffSize, size_t &size);
		void SetPosition(float x, float y);
		void *GetAccessToGLShape() {return ((void*)shape);};
		bool IsModdingEnabled() {return (textModdingEnabled);};
		void SetTextureData(float x, float y, float width, float height, float angle);
		void SetTransformType(int tType) {transformType = tType;};
		void Draw() override;
};

void InitStructure(Shader *usedShader);

#endif
