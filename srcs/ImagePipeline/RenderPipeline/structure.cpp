
#include "structure.h"

Shader *defaultStructureShader = NULL;

float Structure::GetLowY()
{
	t_BoundingB data = shape->GetRotatedBoundingBox();
	float d_drawY = data.leftBottom.y;
	if (d_drawY > data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY > data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY > data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

Structure::Structure(t_DataForShape &data, GLuint texture, int layer, bool textModding)
{
	if (!textModding)
		shape = new GLShape(data.vertexData, data.indexData, texture, defaultStructureShader, data.bBox, 0);
	else
		shape = new GLShapeEX(data.vertexData, data.indexData, texture, defaultStructureShader, data.bBox, 0);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	textModdingEnabled = textModding;
}

void Structure::SetTextureData(float x, float y, float distance, float angle)
{
	if (textModdingEnabled == false)
		return ;
	void *used = (void*)shape;
	GLShapeEX *test = (GLShapeEX*)used;
	test->SetAll(x, y, distance, angle);
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
