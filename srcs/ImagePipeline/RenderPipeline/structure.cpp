
#include "structure.h"

Shader *defaultStructureShader = NULL;

float Structure::GetLowY()
{
	t_BoundingB &data = shape->GetBoundingBox();
	float d_drawY = data.leftBottom.y;
	if (d_drawY > data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY > data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY > data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

Structure::Structure(t_DataForShape &data, GLuint texture, int layer)
{
	shape = new GLShape(data.vertexData, data.indexData, texture, defaultStructureShader, data.bBox, 0);
	drawY = GetLowY();
	AddToRenderSystem(layer);
}

void Structure::Draw()
{
	shape->Draw();
}

Structure::~Structure()
{
	if (shape != NULL)
		delete (shape);
}

void InitStructure(Shader *usedShader)
{
	defaultStructureShader = usedShader;
}
