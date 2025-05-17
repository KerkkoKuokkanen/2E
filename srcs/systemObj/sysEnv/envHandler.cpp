
#include "envHandler.h"
#include "commonTools.h"
#include "renderSystem.h"
#include "keyboard.h"
#include "snapShotCreator.h"
#include "roomLoading.h"
#include <filesystem>
#include <thread>

SysEnv *currentEnvironment = NULL;

static SysEnv *ProtecterCreateSysEnv()
{
	SysEnv *ret = new SysEnv();
	return ret;
}

std::unordered_map<uint64_t, SystemObj*> &GetSysEnvData()
{
	return (currentEnvironment->GetEnvSysObjData());
}

SystemObj *FindSystemObject(uint64_t objKey)
{
	if (currentEnvironment == NULL)
		return (NULL);
	return (currentEnvironment->FindObject(objKey));
}

void UpdateSysEnv()
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->UpdateSysObjects();
	universalRenderingSystem.RenderAll();
	currentEnvironment->LastUpdateSysObjects();
}

void ClearSysEnv()
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->Clear();
}

void DestroyObject(SystemObj *obj)
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->AddToDeleting(obj);
}

void DestroyObject(uint64_t key)
{
	if (currentEnvironment == NULL)
		return ;
	SystemObj *obj = FindSystemObject(key);
	if (obj != NULL)
		currentEnvironment->AddToDeleting(obj);
}

SysEnv *GetCurrentEnvironment()
{
	return (currentEnvironment);
}

uint16_t GetCurrentRoom()
{
	return (1);
}

void ComponentRemover(uint64_t key, uint32_t id)
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->ComponentRemove(key, id);
}

void LoadObjectsToEnvironment(SnapShot snap, uint16_t room)
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->LoadObjects(snap, room);
}

bool LoadEngineRoom()
{
	currentEnvironment = ProtecterCreateSysEnv();
	bool ret = LoadRoomObjects(0);
	return (ret);
}

bool TakeSnapShot()
{
	if (currentEnvironment == NULL)
		return (false);
	bool ret = currentEnvironment->SaveState();
	return (ret);
}

bool QuickSave()
{
	if (currentEnvironment == NULL)
		return (false);
	if (GetSaveData())
		return (false);
	SetSaveData(true);
	return (true);
}

bool FullSave()
{
	if (currentEnvironment == NULL)
		return (false);
	bool ret = TakeSnapShot();
	if (ret == false)
		return (false);
	ret = QuickSave();
	if (ret == false)
		return (false);
	return (true);
}

void CreateNewRoom(std::string name)
{
	if (name == "")
		return ;
	std::filesystem::path room = "saves/rooms/" + name;
	std::filesystem::create_directories(room);
}
