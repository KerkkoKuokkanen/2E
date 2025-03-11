
#include "sysSaver.h"
#include "sysEnv.h"
#include "image.h"
#include "structure.h"
#include "componentRegistry.h"

void *SystemSaver::CreateImageComponent(void *data, size_t size)
{
	size_t offset = 0;
	uint8_t *castData = (uint8_t*)data;
	float posX = *(float*)(castData + offset); offset += sizeof(float);
	float posY = *(float*)(castData + offset); offset += sizeof(float);
	float dimX = *(float*)(castData + offset); offset += sizeof(float);
	float dimY = *(float*)(castData + offset); offset += sizeof(float);
	float w = *(float*)(castData + offset); offset += sizeof(float);
	float h = *(float*)(castData + offset); offset += sizeof(float);
	float angle = *(float*)(castData + offset); offset += sizeof(float);
	float r = *(float*)(castData + offset); offset += sizeof(float);
	float g = *(float*)(castData + offset); offset += sizeof(float);
	float b = *(float*)(castData + offset); offset += sizeof(float);
	float a = *(float*)(castData + offset); offset += sizeof(float);
	uint64_t text = *(uint64_t*)(castData + offset); offset += sizeof(uint64_t);
	int tType = *(int*)(castData + offset); offset += sizeof(int);
	int layer = *(int*)(castData + offset); offset += sizeof(int);
	Image *img = new Image(text, {posX, posY, dimX, dimY}, angle, layer);
	img->SetWidth(w);
	img->SetHeight(h);
	img->SetColor(r, g, b, a);
	img->SetTransformType(tType);
	return ((void*)img);
}

void *SystemSaver::CreateStructureComponent(void *data, size_t size)
{
	size_t offset = 0;
	uint8_t *castData = (uint8_t*)data;
	float posX = *(float*)(castData + offset); offset += sizeof(float);
	float posY = *(float*)(castData + offset); offset += sizeof(float);
	float angle = *(float*)(castData + offset); offset += sizeof(float);
	uint64_t text = *(uint64_t*)(castData + offset); offset += sizeof(uint64_t);
	uint64_t shape = *(uint64_t*)(castData + offset); offset += sizeof(uint64_t);
	int layer = *(int*)(castData + offset); offset += sizeof(int);
	Structure *str = new Structure(shape, text, layer, false);
	str->SetPosition(posX, posY);
	str->SetAngle(angle);
	return ((void*)str);
}

void SystemSaver::CreateComponentForSystemObject(SystemObj *obj, void *data, uint32_t type, size_t size)
{
	switch(type)
	{
		case n_ComponentTypes::NO_CLASS:
		{
			break ;
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			void *str = CreateStructureComponent(data, size);
			obj->AddComponent(str, type);
			break ;
		}
		case n_ComponentTypes::IMAGE_CLASS:
		{
			void *img = CreateImageComponent(data, size);
			obj->AddComponent(img, type);
			break ;
		}
		default :
		{
			std::string used = GetComponentNameWithKey(type);
			obj->AddComponent(used, data, size);
			break ;
		}
	}
}

SystemObj *SystemSaver::GetSystemObjectFromData(void *data, sysKeyObj &store)
{
	if (data == NULL)
		return (NULL);
	uint8_t *castData = (uint8_t*)data;
	uint64_t objectKey = *(uint64_t*)castData;
	int saveable = castData[12];
	SystemObj *ret = new SystemObj();
	ret->saveable = saveable;
	size_t iterator = 0;
	size_t blockSize = *(uint32_t*)(castData + 16);
	uint8_t *compStart = (uint8_t*)(castData + 20);
	while (iterator < blockSize)
	{
		uint32_t type = *(uint32_t*)(compStart + iterator);
		size_t compSize = *(uint32_t*)(compStart + iterator + 4);
		uint8_t *compDataBlock = (uint8_t*)(compStart + iterator + 8);
		CreateComponentForSystemObject(ret, (void*)compDataBlock, type, compSize);
		iterator += (size_t)(compSize + 8);
	}
	store.push_back({objectKey, ret});
	return (ret);
}

SnapShot SystemSaver::CollectLatestSnapshot()
{
	SnapShot ret = {0, 0, NULL};
	if (snapShots.size() == 0)
		return (ret);
	ret = snapShots[currentSnapIndex];
	return (ret);
}

std::vector<std::tuple<uint64_t, SystemObj*>> SystemSaver::LoadSnapShot(SnapShot snapShot)
{
	sysKeyObj ret = {};
	SnapShot *snap = &snapShot;
	if (snap == NULL)
		return (ret);
	size_t iterator = 0;
	uint8_t *data = (uint8_t*)snap->data;
	while (iterator < (size_t)snap->size)
	{
		GetSystemObjectFromData(data + iterator, ret);
		size_t blockSize = *(uint32_t*)(data + iterator + 16);
		iterator += 20 + blockSize;
	}
	return (ret);
}

std::vector<std::tuple<uint64_t, SystemObj*>> SystemSaver::LoadSnapShot(int use)
{
	sysKeyObj ret = {};
	SnapShot *snap = GetSnapShotWithParameter(use);
	if (snap == NULL)
		return (ret);
	size_t iterator = 0;
	uint8_t *data = (uint8_t*)snap->data;
	while (iterator < (size_t)snap->size)
	{
		GetSystemObjectFromData(data + iterator, ret);
		size_t blockSize = *(uint32_t*)(data + iterator + 16);
		iterator += 20 + blockSize;
	}
	return (ret);
}

SnapShot *SystemSaver::GetSnapShotWithParameter(int parameter)
{
	SnapShot *snap = NULL;
	if (snapShots.size() == 0)
		return (snap);
	if (parameter == SNAPSHOT_PREVIOUS)
	{
		if (currentSnapIndex > 0)
		{
			currentSnapIndex -= 1;
			snap = &snapShots[currentSnapIndex];
		}
	}
	else if (parameter == SNAPSHOT_NEXT)
	{
		if (currentSnapIndex == (snapShots.size() - 1))
			return (snap);
		currentSnapIndex += 1;
		snap = &snapShots[currentSnapIndex];
	}
	return (snap);
}
