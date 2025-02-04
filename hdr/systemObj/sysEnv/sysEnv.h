
#ifndef SYS_ENV_H
# define SYS_ENV_H

# include "systemObj.h"
# include "sysSaver.h"
# include <unordered_map>

class SysEnv;
static SysEnv* ProtecterCreateSysEnv();

class SysEnv
{
	private:
		SystemSaver *envState;
		std::unordered_map<uint64_t, SystemObj*> envSysObjs;
		void SnapLoading(sysKeyObj keyObj);
		SysEnv();
		friend SysEnv* ProtecterCreateSysEnv();
	public:
		~SysEnv();
		std::unordered_map<uint64_t, SystemObj*> &GetEnvSysObjData() {return(envSysObjs);};
		void SaveToFile(const char *file);
		void DeleteObject(uint64_t key);
		void RemoveObject(SystemObj *remove);
		void AddObject(SystemObj *add);
		void SaveState();
		void LoadSaveFile(SnapShot &snap);
		void LoadBack(int parameter);
		void Clear();
		SystemObj *FindObject(uint64_t key);
		void UpdateSysObjects();
};

bool EngineModeOn();
void ChangeEngineMode(bool change);

#endif
