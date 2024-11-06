
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
	public:
		Structure(t_DataForShape &data, GLuint texture, int layer, bool textModding);
		~Structure();
		void *GetAccessToGLShape() {return ((void*)shape);};
		void SetTextureData(float x, float y, float distance, float angle);
		void Draw() override;
};

void InitStructure(Shader *usedShader);

#endif
