
#include "sysEnv.h"

SystemObj *SysEnv::FindObject(uint32_t key)
{
	auto obj = envSysObjs.find(key);
	if (obj == envSysObjs.end())
		return (NULL);
	return (obj->second);
}

void SysEnv::UpdateSysObjects()
{
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		current->UpdateSystemObj();
	}
}

SysEnv::SysEnv()
{
	envState = new SystemSaver();
}

SysEnv::~SysEnv()
{
	delete envState;
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		delete (current);
	}
}

void SysEnv::RemoveObject(SystemObj *obj)
{
	if (obj == NULL)
		return ;
	envState->RemoveObjectFromSaver(obj);
	uint32_t key = obj->GetSystemObjectKey();
	envSysObjs.erase(key);
}

void SysEnv::AddObject(SystemObj *obj)
{
	if (obj == NULL)
		return ;
	uint32_t key = obj->GetSystemObjectKey();
	if (envSysObjs.find(key) != envSysObjs.end())
		return ;
	envSysObjs[key] = obj;
}
