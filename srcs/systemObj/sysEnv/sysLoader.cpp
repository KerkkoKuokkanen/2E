
#include "sysSaver.h"
#include "sysEnv.h"
#include "componentRegistry.h"

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
			break ;
		}
		case n_ComponentTypes::IMAGE_CLASS:
		{
			break ;
		}
		default :
		{
			std::string used = GetComponentNameWithKey(type);
			obj->AddComponentCustom(used, data, size);
			break ;
		}
	}
}

SystemObj SystemSaver::*GetSystemObjectFromData(void *data, size_t size, void *controller)
{
	if (controller == NULL)
		return (NULL);
	if (data == NULL)
		return (NULL);
	SysEnv *env = (SysEnv*)controller;
	uint8_t *castData = (uint8_t*)data;
	uint32_t objectKey = *(uint32_t*)castData;
	env->RemoveObject(env->FindObject(objectKey));
	uint8_t saveable = castData[8];
	SystemObj *ret = new SystemObj(controller);
	ret->saveable = saveable;
}
