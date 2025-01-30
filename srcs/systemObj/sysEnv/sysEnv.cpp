
#include "sysEnv.h"
#include "saveInterface.h"

bool engineMode = true;

void ChangeEngineMode(bool change)
{
	engineMode = change;
}

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
		if (current->saveable || engineMode)
			envState->SaveSystemObj(current);
	}
}

void SysEnv::DeleteObject(uint64_t key)
{
	auto eobj = envSysObjs.find(key);
	if (eobj == envSysObjs.end())
		return ;
	envState->RemoveObjectFromSaver(eobj->second);
	eobj->second->controller = NULL;
	delete eobj->second;
	envSysObjs.erase(key);
}

void SysEnv::SnapLoading(sysKeyObj keyObj)
{
	if (keyObj.size() == 0)
		return ;
	/* for (auto it = envSysObjs.begin(); it != envSysObjs.end();)
	{
		if (it->second->saveable)
		{
			uint64_t key = it->first;
			it++;
			this->DeleteObject(key);
		}
		else
			it++;
	} */
	for (int i = 0; i < keyObj.size(); i++)
	{
		uint64_t key = std::get<0>(keyObj[i]);
		SystemObj *obj = std::get<1>(keyObj[i]);
		obj->SetUniqueKeyManual(key);
		this->AddObject(obj);
	}
}

void SysEnv::LoadSaveFile(SnapShot &snap)
{
	sysKeyObj ret = envState->LoadSnapShot(snap);
	SnapLoading(ret);
}

void SysEnv::LoadBack(int parameter)
{
	sysKeyObj ret = envState->LoadSnapShot(parameter);
	SnapLoading(ret);
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

void SysEnv::SaveToFile()
{
	envState->TakeSnapShot();
	SnapShot save = envState->CollectLatestSnapshot();
	std::string chosenFile = currentFile + "er0.2E";
	SaveSnapShot(save, chosenFile);
}
//comment