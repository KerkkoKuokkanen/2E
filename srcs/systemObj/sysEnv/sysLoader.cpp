
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
	float angle = *(float*)(castData + offset); offset += sizeof(float);
	GLuint text = *(GLuint*)(castData + offset); offset += sizeof(GLuint);
	int layer = *(int*)(castData + offset); offset += sizeof(int);
	Image *img = new Image(text, {posX, posY, dimX, dimY}, angle, layer);
	return ((void*)img);
}

void *SystemSaver::CreateStructureComponent(void *data, size_t size)
{
	size_t offset = 0;
	uint8_t *castData = (uint8_t*)data;
	float posX = *(float*)(castData + offset); offset += sizeof(float);
	float posY = *(float*)(castData + offset); offset += sizeof(float);
	float angle = *(float*)(castData + offset); offset += sizeof(float);
	GLuint text = *(GLuint*)(castData + offset); offset += sizeof(GLuint);
	GLuint shape = *(GLuint*)(castData + offset); offset += sizeof(GLuint);
	int layer = *(int*)(castData + offset); offset += sizeof(int);
	Structure *str = new Structure(shape, text, layer, false);
	str->SetPosition(posX, posY);
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
			obj->AddComponent(data, type);
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
	uint8_t saveable = castData[12];
	SystemObj *ret = new SystemObj();
	ret->saveable = saveable;
	size_t iterator = 0;
	size_t blockSize = *(uint32_t*)(castData + 13);
	uint8_t *compStart = (uint8_t*)(castData + 17);
	while (iterator < blockSize)
	{
		uint32_t type = *(uint32_t*)(compStart);
		size_t compSize = *(uint32_t*)(compStart + 4);
		uint8_t *compDataBlock = (uint8_t*)(compStart + 8);
		CreateComponentForSystemObject(ret, (void*)compDataBlock, type, compSize);
		iterator += compSize + 8;
	}
	store.push_back({objectKey, ret});
	return (ret);
}

SnapShot *SystemSaver::GetSnapShotWithParameter(int parameter)
{
	if (snapShots.size() == 0)
		return (NULL);
	return (&snapShots[snapShots.size() - 1]);
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
		GetSystemObjectFromData((void*)(data + iterator), ret);
		size_t blockSize = *(uint32_t*)(data + iterator + 13);
		iterator += 17 + blockSize;
	}
	return (ret);
}
