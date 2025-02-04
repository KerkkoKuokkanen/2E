
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

bool LoadEngineRoom()
{
	currentEnvironment = ProtecterCreateSysEnv();
	uint32_t key = SetAskedData("saves/rooms/engineRoom/er0.2E");
	void *state = NULL;
	for (int i = 0; i < 15; i++)
	{
		bool checker = CollectAskedState(key, &state);
		if (checker)
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
	currentEnvironment->SaveToFile("saves/rooms/engineRoom/er0.2E");
	return (true);
}
