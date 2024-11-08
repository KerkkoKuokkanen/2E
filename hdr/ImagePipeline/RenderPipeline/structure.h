
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
		t_Point position = {0.0f, 0.0f};
		int transformType = n_TransformTypes::TRANSFORM_CAMERA;
	public:
		Structure(t_DataForShape &data, GLuint texture, int layer, bool textModding);
		~Structure();
		void SetPosition(float x, float y);
		void *GetAccessToGLShape() {return ((void*)shape);};
		bool IsModdingEnabled() {return (textModdingEnabled);};
		void SetTextureData(float x, float y, float distance, float angle);
		void SetTransformType(int tType) {transformType = tType;};
		void Draw() override;
};

void InitStructure(Shader *usedShader);

#endif
