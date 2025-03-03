
#ifndef ENV_HANDLER_H
# define ENV_HANDLER_H

# include "sysEnv.h"

bool LoadEngineRoom();
bool SaveEngineRoom();
SysEnv *GetCurrentEnvironment();
void UpdateSysEnv();
void ClearSysEnv();
void DeleteObject(uint64_t key);
void DestroyObject(SystemObj *obj);
std::unordered_map<uint64_t, SystemObj*> &GetSysEnvData();
SystemObj *FindSystemObject(uint64_t objKey);

#endif
