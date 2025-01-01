
#include "transform.h"
#include "vectorTools.h"
#include "commonTools.h"

t_Point Transform::VecHeighDirection()
{
	t_Point normVec = {1.0f, 0.0f};
	t_Point rotated = VectorRotate(normVec, ownAngle);
	return (rotated);
}

t_Point Transform::VecWidthDirection()
{
	t_Point normVec = {1.0f, 0.0f};
	t_Point rotated = VectorRotate(normVec, ownAngle);
	t_Point last = VectorRotate(rotated, PI / 2.0f);
	return (last);
}

void Transform::Width(float widht)
{
	t_Point scaleDir = VecWidthDirection();
	for (int i = 0; i < images.size(); i++)
	{
		GLShape *shape = images[i]->GetAccessToGLSprite()->shape;
		shape->SetDirScaleDir(scaleDir);
		shape->SetDirScaleWidth(widht);
	}
	for (int i = 0; i < structures.size(); i++)
	{
		GLShape *shape = (GLShape*)structures[i]->GetAccessToGLShape();
		shape->SetDirScaleDir(scaleDir);
		shape->SetDirScaleWidth(widht);
	}
	ownWidth = widht;
}

void Transform::Height(float height)
{
	t_Point scaleDir = VecHeighDirection();
	for (int i = 0; i < images.size(); i++)
	{
		GLShape *shape = images[i]->GetAccessToGLSprite()->shape;
		shape->SetDirScaleDir(scaleDir);
		shape->SetDirScaleHeight(height);
	}
	for (int i = 0; i < structures.size(); i++)
	{
		GLShape *shape = (GLShape*)structures[i]->GetAccessToGLShape();
		shape->SetDirScaleDir(scaleDir);
		shape->SetDirScaleHeight(height);
	}
	ownHeight = height;
}
