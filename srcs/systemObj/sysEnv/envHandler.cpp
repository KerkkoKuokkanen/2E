
#include "envHandler.h"
#include "commonTools.h"
#include "renderSystem.h"
#include "keyboard.h"
#include "snapShotCreator.h"
#include "roomLoading.h"
#include <filesystem>
#include <thread>

SysEnv *currentEnvironment = NULL;

int switchRoom = -1;
uint16_t currentRoom = 1;
bool controlZ = false;
std::vector<uint16_t> additionalLoads = {};

bool GetControlZ()
{
	return (controlZ);
}

void SetCurrentRoom(uint16_t room)
{
	currentRoom = room;
}

uint16_t GetCurrentRoom()
{
	return (currentRoom);
}

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

void *FindAny(std::string component)
{
	if (currentEnvironment == NULL)
		return (NULL);
	return (currentEnvironment->FindAny(component));
}

void DoTheRoomSwithc()
{
	if (switchRoom < 0)
		return ;
	ClearSysEnv();
	LoadRoom(switchRoom);
	currentRoom = switchRoom;
	for (uint16_t room : additionalLoads)
		LoadRoom(room);
	switchRoom = -1;
	additionalLoads.clear();
}

void UpdateSysEnv()
{
	if (currentEnvironment == NULL)
		return ;
	controlZ = false;
	if (EngineModeOn())
		ControlZ(&controlZ);
	currentEnvironment->UpdateSysObjects();
	universalRenderingSystem.RenderAll();
	currentEnvironment->LastUpdateSysObjects();
	DoTheRoomSwithc();
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

void RoomSwitch(uint16_t room, std::vector<uint16_t> loaded)
{
	switchRoom = room;
	for (uint16_t add : loaded)
		additionalLoads.push_back(add);
}

bool LoadRoom(uint16_t room)
{
	bool ret = LoadRoomObjects(room);
	return (ret);
}

bool LoadEngineRoom()
{
	currentEnvironment = ProtecterCreateSysEnv();
	bool ret = LoadRoomObjects(0);
	if (ret == false)
		return (false);
	ret = LoadRoom(GetCurrentRoom());
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
	AddNewRoom(name);
}
