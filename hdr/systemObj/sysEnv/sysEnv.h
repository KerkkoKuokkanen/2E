
#ifndef SYS_ENV_H
# define SYS_ENV_H

# include "systemObj.h"
# include "sysSaver.h"
# include <unordered_map>

class SysEnv
{
	private:
		SystemSaver *saver;
		std::unordered_map<uint32_t, SystemObj*> envSysObjs;
	public:
		SysEnv();
		~SysEnv();
		void RemoveObject(SystemObj *remove);
		void AddObject(SystemObj *add);
		SystemObj *FindObject(uint32_t key);
		void UpdateSysObjects();
};

#endif
