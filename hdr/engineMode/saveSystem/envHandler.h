
#ifndef ENV_HANDLER_H
# define ENV_HANDLER_H

# include "sysEnv.h"

bool LoadEngineRoom();
bool SaveEngineRoom();
SysEnv *GetCurrentEnvironment();
void UpdateSysEnv();

#endif
