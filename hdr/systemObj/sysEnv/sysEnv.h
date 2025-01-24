
#ifndef SYS_ENV_H
# define SYS_ENV_H

# include "systemObj.h"
# include "sysSaver.h"
# include <unordered_map>

class SysEnv
{
	private:
		SystemSaver *envState;
		std::unordered_map<uint64_t, SystemObj*> envSysObjs;
		std::string currentFile = "saves/rooms/engineRoom/";
		int roomCycle = 0;
	public:
		SysEnv();
		~SysEnv();
		void SaveToFile();
		void DeleteObject(uint64_t key);
		void RemoveObject(SystemObj *remove);
		void AddObject(SystemObj *add);
		void SaveState();
		void LoadBack(int parameter);
		SystemObj *FindObject(uint64_t key);
		void UpdateSysObjects();
};

#endif
