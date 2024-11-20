
#include "sysEnv.h"

static void *ResizeData(void *data, size_t newSize)
{
	data = realloc(data, newSize);
	return (data);
}

bool SysEnv::SysObjComponentFetch(SystemObj *obj)
{
	size_t gottenSize = 0;
	void *ret = obj->FetchComponentSaveData(dataFetcher, dataFetcherSize, gottenSize);
	if (ret == NULL)
	{
		printf("/sysEnvSave.cpp: size too large\n");
		return (false);
	}
	
}

void *SysEnv::CollectSaveDeltaChange()
{
	size_t total = 0;
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		if (current->saveable == false)
			continue ;
		current->ResetComponentSaveFetching();
		while (current->ComponentFetchingAtEnd() == false)
		{
			if (SysObjComponentFetch(current) == false)
				continue ;
		}

	}
}
