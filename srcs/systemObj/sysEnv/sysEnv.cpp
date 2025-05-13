
#include "sysEnv.h"
#include "saveInterface.h"
#include "keyboard.h"

#define BUILD_MODE false

bool engineMode = true;
bool overImgui = false;

bool EngineModeOn()
{
	return (engineMode);
}

void ChangeEngineMode(bool change)
{
	engineMode = change;
}

bool OverImgui()
{
	return (overImgui);
}

void ChangeOverImgui(bool change)
{
	overImgui = change;
}

SystemObj *SysEnv::FindObject(uint64_t key)
{
	auto obj = envSysObjs.find(key);
	if (obj == envSysObjs.end())
		return (NULL);
	return (obj->second);
}

void SysEnv::LastUpdateSysObjects()
{
	std::vector<SystemObj*> objs = {};
	objs.reserve(envSysObjs.size());
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		objs.push_back(current);
	}
	std::sort(objs.begin(), objs.end(),
				[](const SystemObj *a, const SystemObj *b) {
					return a->weight < b->weight;
				});
	for (SystemObj *obj : objs)
	{
		obj->LastUpdateSystemObj();
		if ((obj->saveable != 0 || engineMode) && obj->forceNoSave == false)
			envState->SaveSystemObj(obj);
	}
	for (int i = 0; i < compDeleting.size(); i++)
	{
		envState->changeSpotted = true;
		envState->RemoveComponentFromSaver(std::get<0>(compDeleting[i]), std::get<1>(compDeleting[i]));
	}
	for (int i = 0; i < deleting.size(); i++)
	{
		envState->changeSpotted = true;
		if (this->DeleteObject(deleting[i]->FetchComponentUniqueKey()) == false)
			delete deleting[i];
	}
	deleting.clear();
	if (envState->changeSpotted && loaded == false)
		SaveState();
	loaded = false;
	if (!BUILD_MODE && KeyHeld(SDL_SCANCODE_LCTRL) && KeyPressed(SDL_SCANCODE_P))
		engineMode = true;
}

void SysEnv::UpdateSysObjects()
{
	envState->changeSpotted = false;
	std::vector<SystemObj*> objs = {};
	objs.reserve(envSysObjs.size());
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		objs.push_back(current);
	}
	std::sort(objs.begin(), objs.end(),
				[](const SystemObj *a, const SystemObj *b) {
					return a->weight < b->weight;
				});
	for (SystemObj *obj : objs)
		obj->UpdateSystemObj();
}

bool SysEnv::DeleteObject(uint64_t key)
{
	auto eobj = envSysObjs.find(key);
	if (eobj == envSysObjs.end())
		return (false);
	envState->RemoveObjectFromSaver(eobj->second);
	eobj->second->controller = NULL;
	delete eobj->second;
	envSysObjs.erase(key);
	return (true);
}

void SysEnv::SnapLoading(sysKeyObj keyObj)
{
	if (keyObj.size() == 0)
		return ;
	for (int i = 0; i < keyObj.size(); i++)
	{
		uint64_t key = std::get<0>(keyObj[i]);
		SystemObj *obj = std::get<1>(keyObj[i]);
		RemoveObject(obj);
		obj->SetUniqueKeyManual(key);
		AddObject(obj);
	}
}

void SysEnv::LoadSaveFile(SnapShot &snap)
{
	Clear();
	sysKeyObj ret = envState->LoadSnapShot(snap);
	SnapLoading(ret);
}

void SysEnv::ChangeRoom(SnapShot &snap)
{

}

void SysEnv::LoadBack(int parameter)
{
	if (envState->currentSnapIndex == 0)
		return ;
	Clear();
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

void SysEnv::ClearRoom()
{
	std::vector<SystemObj*> presists = {};
	for (const auto &[key, obj] : envSysObjs)
	{
		obj->RoomChangeUpdate();
		if (obj->presist)
		{
			obj->presist = false;
			presists.push_back(obj);
			continue ;
		}
		envState->RemoveObjectFromSaver(obj);
		obj->controller = NULL;
		delete obj;
	}
	envSysObjs.clear();
	for (int i = 0; i < presists.size(); i++)
		envSysObjs[presists[i]->GetSystemObjectKey()] = presists[i];
}

void SysEnv::Clear()
{
	for (const auto &[key, obj] : envSysObjs)
	{
		envState->RemoveObjectFromSaver(obj);
		obj->controller = NULL;
		delete obj;
	}
	envSysObjs.clear();
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

void SysEnv::SaveToFile(const char *file)
{
	envState->TakeSnapShot();
	SnapShot save = envState->CollectLatestSnapshot();
	SaveSnapShot(save, file);
}
