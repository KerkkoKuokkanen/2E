
#ifndef ENV_HANDLER_H
# define ENV_HANDLER_H

# include "sysEnv.h"

bool LoadEngineRoom();
bool SaveEngineRoom();
SysEnv *GetCurrentEnvironment();
void UpdateSysEnv();
void ClearSysEnv();
void DeleteObject(uint64_t key);
std::unordered_map<uint64_t, SystemObj*> &GetSysEnvData();

#endif
