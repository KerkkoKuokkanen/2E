
#include "sysEnv.h"

SystemObj *SysEnv::FindObject(uint64_t key)
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
		envState->SaveSystemObj(current);
	}
}

void SysEnv::DeleteObject(uint64_t key)
{
	auto eobj = envSysObjs.find(key);
	if (eobj == envSysObjs.end())
		return ;
	static int loop = 0;
	envState->RemoveObjectFromSaver(eobj->second);
	eobj->second->controller = NULL;
	delete eobj->second;
	envSysObjs.erase(key);
	loop += 1;
}

void SysEnv::LoadBack()
{
	static int loop = 0;
	sysKeyObj ret = envState->LoadSnapShot(0);
	for (int i = 0; i < ret.size(); i++)
	{
		uint64_t key = std::get<0>(ret[i]);
		SystemObj *obj = std::get<1>(ret[i]);
		this->DeleteObject(key);
		obj->SetUniqueKeyManual(key);
		this->AddObject(obj);
	}
	loop++;
}

void SysEnv::SaveState()
{
	envState->TakeSnapShot();
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
	uint64_t key = obj->GetSystemObjectKey();
	auto eobj = envSysObjs.find(key);
	if (eobj == envSysObjs.end())
		return ;
	envSysObjs.erase(key);
}

void SysEnv::AddObject(SystemObj *obj)
{
	if (obj == NULL)
		return ;
	uint64_t key = obj->GetSystemObjectKey();
	if (envSysObjs.find(key) != envSysObjs.end())
		return ;
	envSysObjs[key] = obj;
}
