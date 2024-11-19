
#ifndef SYS_ENV_H
# define SYS_ENV_H

# include "systemObj.h"
# include "memoryPool.h"
# include <unordered_map>

class SysEnv
{
	private:
		MemoryPool *customObjPool = NULL;
		std::unordered_map<unsigned int, SystemObj*> envSysObjs;
	public:
		~SysEnv();
		void *CollectSaveData(size_t *size);
		SystemObj *FindObject(unsigned int key);
		void UpdateSysObjects();
};

#endif
