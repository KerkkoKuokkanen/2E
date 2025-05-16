
#include "envHandler.h"
#include "saveInterface.h"
#include "commonTools.h"
#include "renderSystem.h"
#include "keyboard.h"
#include "snapShotCreator.h"
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

void DeleteObject(uint64_t key)
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->DeleteObject(key);
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

bool LoadEngineRoom()
{
	currentEnvironment = ProtecterCreateSysEnv();
	uint32_t key = SetAskedData("saves/rooms/engineRoom/er0.2E");
	void *state = NULL;
	for (int i = 0; i < 60; i++)
	{
		bool checker = CollectAskedState(key, &state);
		if (checker)
			break ;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (state == NULL)
		return (true);
	SnapShot data = MakeIntoSnapshot(state);
	currentEnvironment->Clear();
	currentEnvironment->LoadObjects(data);
	free(state);
	free(data.data);
	return (true);
}

bool LoadRoom(const char *str)
{
	uint32_t key = SetAskedData("saves/rooms/engineRoom/er0.2E");
	void *state = NULL;
	for (int i = 0; i < 60; i++)
	{
		bool checker = CollectAskedState(key, &state);
		if (checker)
			break ;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (state == NULL)
		return (true);
	SnapShot data = MakeIntoSnapshot(state);
	currentEnvironment->Clear();
	currentEnvironment->LoadObjects(data);
	free(state);
	free(data.data);
	return (true);
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
