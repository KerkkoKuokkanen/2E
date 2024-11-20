
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

Structure::Structure(GLuint sshape, GLuint texture, int layer, bool textModding)
{
	t_DataForShape &data = GetShapeDataWithKey(sshape);
	if (!textModding)
		shape = new GLShape(data.vertexData, data.indexData, texture, defaultStructureShader, data.bBox, 0);
	else
		shape = new GLShapeEX(data.vertexData, data.indexData, texture, defaultStructureShader, data.bBox, 0);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	textModdingEnabled = textModding;
	position = {0.0f, 0.0f};
	Structure::texture = texture;
	shapeData = sshape;
}

void Structure::SetPosition(float x, float y)
{
	position = {x, y};
}

void Structure::SetTextureData(float x, float y, float width, float height, float angle)
{
	if (textModdingEnabled == false)
		return ;
	GLShapeEX *cast = (GLShapeEX*)shape;
	cast->SetAll(x, y, width, height, angle);
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
	drawY = GetLowY();
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

void *Structure::CollectSaveData(void *buffer, size_t buffSize, size_t &size)
{
	if (saveable == false)
		return (NULL);
	size_t dataSize = sizeof(float) * 3 + sizeof(GLuint) * 2 + sizeof(int);
	size = dataSize;
	if (dataSize > buffSize)
		return (NULL);
	char *byteData = (char *)buffer;
	size_t offset = 0;
	memcpy(byteData + offset, &position.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &position.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &angle, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &texture, sizeof(GLuint)); offset += sizeof(GLuint);
	memcpy(byteData + offset, &shapeData, sizeof(GLuint)); offset += sizeof(GLuint);
	memcpy(byteData + offset, &layer, sizeof(int));

	return (buffer);
}
