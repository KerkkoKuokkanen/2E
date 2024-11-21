
#include "sysSaver.h"
#include "commonTools.h"

void SystemSaver::AddNewObject(SystemObj *add)
{
	add->ResetComponentSaveFetching();
	SaveObj newAddition;
	while (add->ComponentFetchingAtEnd() == false)
	{
		SaveObjData addition;
		size_t sizerTool = 0;
		addition.compSize = add->FetchComponentDataSize();
		if (addition.compSize == 0)
		{
			addition.data = NULL;
			addition.hash = 0;
			add->IncrimentComponentFetching();
			continue ;
		}
		addition.componentKey = add->FetchComponentUniqueKey();
		addition.data = malloc(addition.compSize);
		void *ret = add->FetchComponentSaveData(addition.data, addition.compSize, sizerTool);
		if (ret == NULL)
			printf("SystemSaver.cpp: AddNewObject ret == NULL\n");
		addition.data = ret;
		addition.hash = HashData32(ret, addition.compSize);
		newAddition.components.push_back(addition);
		newAddition.objHash ^= addition.hash;
		add->IncrimentComponentFetching();
	}
	objectSaves[add->GetSystemObjectKey()] = newAddition;
}

void SystemSaver::SaveSystemObj(SystemObj *save)
{
	uint32_t key = save->GetSystemObjectKey();
	if (objectSaves.find(key) == objectSaves.end())
		AddNewObject(save);
}

void SystemSaver::RemoveObjectFromSaver(SystemObj *obj)
{
	uint32_t key = obj->GetSystemObjectKey();
	auto saveObj = objectSaves.find(key);
	if (saveObj == objectSaves.end())
		return ;
	SaveObj &delObj = saveObj->second;
	for (int i = 0; i < delObj.components.size(); i++)
	{
		if (delObj.components[i].data != NULL)
			free(delObj.components[i].data);
	}
	objectSaves.erase(key);
}

SystemSaver::SystemSaver()
{
	dataFetcher = malloc(FETCH_SIZE);
}

SystemSaver::~SystemSaver()
{
	free(dataFetcher);
	for (auto &[key, save] : objectSaves)
	{
		SaveObj &obj = save;
		for (int i = 0; i < obj.components.size(); i++)
		{
			if (obj.components[i].data != NULL)
				free(obj.components[i].data);
		}
	}
}
