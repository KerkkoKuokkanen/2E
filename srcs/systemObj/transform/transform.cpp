
#include "transform.h"
#include "vectorTools.h"
#include "commonTools.h"

void Transform::RemoveImage(Image *img)
{
	if (img == NULL)
		return ;
	uint32_t id = img->id;
	for (int i = 0; i < images.size(); i++)
	{
		if (images[i]->id == id)
		{
			images.erase(images.begin() + i);
			return ;
		}
	}
}

void Transform::RemoveStructure(Structure *str)
{
	if (str == NULL)
		return ;
	uint32_t id = str->id;
	for (int i = 0; i < structures.size(); i++)
	{
		if (structures[i]->id == id)
		{
			structures.erase(structures.begin() + i);
			return ;
		}
	}
}

t_Point Transform::GetAngleForObj(t_Point pos, float angle)
{
	t_Point offSet = {pos.x - ownPosition.x, pos.y - ownPosition.y};
	t_Point rotated = VectorRotate(offSet, -angle);
	t_Point ret = {ownPosition.x + rotated.x, ownPosition.y + rotated.y};
	return (ret);
}

void Transform::Angle(float angle)
{
	if (FAlmostEqual(ownAngle, angle, 0.00001f))
		return ;
	float angleAdd = ownAngle - angle;
	ownAngle = angle;
	for (int i = 0; i < images.size(); i++)
	{
		float sAngle = images[i]->GetAngle();
		t_Point pos = images[i]->GetPosition();
		pos = GetAngleForObj(pos, angleAdd);
		images[i]->SetPosition(pos.x, pos.y);
		GLSprite *sprite = images[i]->GetAccessToGLSprite();
		sprite->SetRotation(sAngle + ownAngle);
	}
	for (int i = 0; i < structures.size(); i++)
	{
		float sAngle = structures[i]->GetAngle();
		t_Point pos = structures[i]->GetPosition();
		pos = GetAngleForObj(pos, angleAdd);
		structures[i]->SetPosition(pos.x, pos.y);
		GLShape *shape = (GLShape*)structures[i]->GetAccessToGLShape();
		shape->SetRotation(sAngle + ownAngle);
	}
}

void Transform::Position(float x, float y)
{
	for (int i = 0; i < images.size(); i++)
	{
		t_Point pos = images[i]->GetPosition();
		t_Point offset = {pos.x - ownPosition.x, pos.y - ownPosition.y};
		images[i]->SetPosition(x + offset.x, y + offset.y);
	}
	for (int i = 0; i < structures.size(); i++)
	{
		t_Point pos = structures[i]->GetPosition();
		t_Point offset = {pos.x - ownPosition.x, pos.y - ownPosition.y};
		structures[i]->SetPosition(x + offset.x, y + offset.y);
	}
	ownPosition = {x, y};
}

size_t Transform::SaveDataSize()
{
	size_t ret = sizeof(float) * 5;
	return (ret);
}

void *Transform::CollectSaveData(void *buffer, size_t buffSize, size_t &size)
{
	size_t dataSize = sizeof(float) * 5;
	size = dataSize;
	if (dataSize > buffSize)
		return (NULL);
	uint8_t *byteData = (uint8_t *)buffer;
	size_t offset = 0;
	memcpy(byteData + offset, &ownPosition.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &ownPosition.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &ownWidth, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &ownHeight, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &ownAngle, sizeof(float)); offset += sizeof(float);
	return (buffer);
}
