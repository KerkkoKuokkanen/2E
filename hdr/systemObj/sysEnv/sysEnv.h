
#ifndef SYS_ENV_H
# define SYS_ENV_H

# include "systemObj.h"
# include <unordered_map>

# define SYS_ENV_MB_SIZE 1048576

struct SysComponentData
{
	uint32_t hash;
	size_t size;
	void *data;
};

class SysEnv
{
	private:
		void *dataFetcher = NULL;
		size_t dataFetcherSize = 0;
		std::unordered_map<uint32_t, SystemObj*> envSysObjs;
		std::unordered_map<uint32_t, std::vector<SysComponentData>> envObjData;
		bool SysObjComponentFetch(SystemObj *obj);
	public:
		SysEnv();
		~SysEnv();
		void *CollectSaveDeltaChange();
		void RemoveObject(SystemObj *remove);
		void AddObject(SystemObj *add);
		SystemObj *FindObject(uint32_t key);
		void UpdateSysObjects();
};

#endif
