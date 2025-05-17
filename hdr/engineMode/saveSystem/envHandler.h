
#ifndef ENV_HANDLER_H
# define ENV_HANDLER_H

# include "sysEnv.h"

bool LoadEngineRoom();

bool TakeSnapShot();
bool QuickSave();
bool FullSave();

SysEnv *GetCurrentEnvironment();
uint16_t GetCurrentRoom();

void UpdateSysEnv();
void ClearSysEnv();
void DestroyObject(SystemObj *obj);
void DestroyObject(uint64_t key);
void LoadObjectsToEnvironment(SnapShot snap, uint16_t room);

std::unordered_map<uint64_t, SystemObj*> &GetSysEnvData();
SystemObj *FindSystemObject(uint64_t objKey);

void ComponentRemover(uint64_t key, uint32_t id);
void CreateNewRoom(std::string name);

#endif
