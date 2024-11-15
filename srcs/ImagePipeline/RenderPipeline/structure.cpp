
#include "structure.h"
#include "imageTransforms.h"

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
	position = {0.0f, 0.0f};
}

void Structure::SetPosition(float x, float y)
{
	position = {x, y};
}

void Structure::SetTextureData(float x, float y, float distance, float angle)
{
	if (textModdingEnabled == false)
		return ;
	GLShapeEX *test = (GLShapeEX*)shape;
	test->SetAll(x, y, distance, angle);
}

void Structure::Draw()
{
	if (shape == NULL)
		return ;
	float x = position.x;
	float y = position.y;
	t_Point used;
	switch (transformType)
	{
		case n_TransformTypes::TRANSFORM_CAMERA:
			used = TransformCoordinateToScreenSpaceCamera(x, y);
			break ;
		case n_TransformTypes::TRANSFORM_STATIC:
			used = TransformCoordinateToScreenSpace(x, y);
		default:
			used = {x, y};
			break ;
	}
	shape->SetPosition(used.x, used.y);
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
