
#include "envHandler.h"
#include "saveInterface.h"
#include "commonTools.h"
#include <thread>

SysEnv *currentEnvironment = NULL;

static SysEnv *ProtecterCreateSysEnv()
{
	SysEnv *ret = new SysEnv();
	return ret;
}

void UpdateSysEnv()
{
	if (currentEnvironment == NULL)
		return ;
	currentEnvironment->UpdateSysObjects();
}

SysEnv *GetCurrentEnvironment()
{
	return (currentEnvironment);
}

static SnapShot MakeIntoSnapshot(void *data)
{
	char *cast = (char*)data;
	uint64_t hash = 0;
	uint32_t size = 0;
	memcpy(&hash, cast, sizeof(uint64_t));
	memcpy(&size, cast + sizeof(uint64_t), sizeof(uint32_t));
	void *use = malloc(size);
	char *useCast = (char*)use;
	memcpy(useCast, cast + sizeof(uint32_t) + sizeof(uint64_t), size);
	uint64_t checkHash = HashData64(use, (size_t)size);
	if (checkHash == hash)
		return ((SnapShot){hash, size, use});
	free(data);
	free(use);
	return ((SnapShot){0, 0, NULL});
}

bool LoadEngineRoom()
{
	currentEnvironment = ProtecterCreateSysEnv();
	SetAskedData("saves/rooms/engineRoom/er0.2E");
	void *state = NULL;
	for (int i = 0; i < 15; i++)
	{
		state = CollectAskedState();
		if (state != NULL)
			break ;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (state == NULL)
		return (true);
	SnapShot data = MakeIntoSnapshot(state);
	currentEnvironment->LoadSaveFile(data);
	free(state);
	free(data.data);
	return (true);
}

bool SaveEngineRoom()
{
	if (currentEnvironment == NULL)
		return (false);
	currentEnvironment->SaveToFile();
	return (true);
}
