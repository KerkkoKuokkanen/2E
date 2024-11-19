
#include "sysEnv.h"

SystemObj *SysEnv::FindObject(unsigned int key)
{
	auto obj = envSysObjs.find(key);
	if (obj == envSysObjs.end())
		return (NULL);
	return (obj->second);
}

void SysEnv::UpdateSysObjects()
{
	for (const auto& [key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		current->UpdateSystemObj();
	}
}

SysEnv::~SysEnv()
{
	for (const auto& [key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		delete (current);
	}
}
