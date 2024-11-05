
#ifndef STRUCTURE_H
# define STRUCTURE_H

# include "shape.h"
# include "renderSystem.h"

class Structure : public RenderObj
{
	private:
		GLShape *shape = NULL;
		float GetLowY();
	public:
		Structure(t_DataForShape &data, GLuint texture, int layer);
		~Structure();
		GLShape *GetAccessToGLShape() {return (shape);};
		void Draw() override;
};

void InitStructure(Shader *usedShader);

#endif
