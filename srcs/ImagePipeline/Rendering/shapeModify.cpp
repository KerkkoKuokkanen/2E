
#include "shape.h"
#include "commonTools.h"

static t_Point GetCenter(t_BoundingB &boundBox)
{
	float xCenter = (boundBox.leftBottom.x + boundBox.leftTop.x + boundBox.rightBottom.x + boundBox.rightTop.x) / 4.0f;
	float yCenter = (boundBox.leftBottom.y + boundBox.leftTop.y + boundBox.rightBottom.y + boundBox.rightTop.y) / 4.0f;
	return ((t_Point){xCenter, yCenter});
}

static float VecMagnitude(t_Point vec)
{
	float mag = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (mag);
}

Vertex *GLShape::BindOwn()
{
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex *vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	return (vertData);
}

void GLShape::UnBindOwn()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLShape::SetHeightOwn(float h, Vertex *vertData)
{
	RotateGLShape(-angle, vertData);
	float boxH = abs(boundBox.leftTop.y - boundBox.leftBottom.y);
	if (FAlmostEqual(boxH, h, 0.000001f))
	{
		RotateGLShape(angle, vertData);
		return ;
	}
	t_Point center = GetCenter(boundBox);
	float add = h - boxH;
	float addToSide = add * 0.5f;
	float centerY = center.y;
	float scale = addToSide / (boxH * 0.5f);
	for (int i = 0; i < vertexAmount; i++)
	{
		float vertY = vertData[i].position.y;
		float distFrombBoxC = vertY - centerY;
		float indAdd = scale * distFrombBoxC;
		vertY += indAdd;
		vertData[i].position.y = vertY;
	}
	boundBox.leftTop.y += addToSide;
	boundBox.rightTop.y += addToSide;
	boundBox.leftBottom.y -= addToSide;
	boundBox.rightBottom.y -= addToSide;
	RotateGLShape(angle, vertData);
}

void GLShape::SetWidthOwn(float w, Vertex *vertData)
{
	RotateGLShape(-angle, vertData);
	float boxW = abs(boundBox.leftTop.x - boundBox.rightTop.x);
	if (FAlmostEqual(boxW, w, 0.000001f))
	{
		RotateGLShape(angle, vertData);
		return ;
	}
	t_Point center = GetCenter(boundBox);
	float add = w - boxW;
	float addToSide = add * 0.5f;
	float centerX = center.x;
	float scale = addToSide / (boxW * 0.5f);
	for (int i = 0; i < vertexAmount; i++)
	{
		float vertX = vertData[i].position.x;
		float distFrombBoxC = vertX - centerX;
		float indAdd = scale * distFrombBoxC;
		vertX += indAdd;
		vertData[i].position.x = vertX;

	}
	boundBox.leftTop.x -= addToSide;
	boundBox.rightTop.x += addToSide;
	boundBox.leftBottom.x -= addToSide;
	boundBox.rightBottom.x += addToSide;
	RotateGLShape(angle, vertData);
}

void GLShape::SetHeight(float h)
{
	if (h <= 0.0f)
		return ;
	Vertex *data = BindOwn();
	SetHeightOwn(h, data);
	UnBindOwn();
}

void GLShape::SetWidth(float w)
{
	if (w <= 0.0f)
		return ;
	Vertex *data = BindOwn();
	SetWidthOwn(w, data);
	UnBindOwn();
}

void GLShape::SetScale(float scale)
{
	if (scale <= 0.0f)
		return ;
	Vertex *data = BindOwn();
	SetHeightOwn(scale, data);
	SetWidthOwn(scale, data);
	UnBindOwn();
}