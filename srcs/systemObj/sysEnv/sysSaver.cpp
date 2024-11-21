
#include "sysSaver.h"
#include "commonTools.h"

void SystemSaver::AddNewComponentToObject(SystemObj *add, SaveObj &newAddition)
{
	SaveObjData addition;
	size_t sizerTool = 0;
	addition.compSize = add->FetchComponentDataSize();
	if (addition.compSize == 0)
		return ;
	addition.componentKey = add->FetchComponentUniqueKey();
	addition.data = malloc(addition.compSize);
	void *ret = add->FetchComponentSaveData(addition.data, addition.compSize, sizerTool);
	if (ret == NULL)
		printf("SystemSaver.cpp: AddNewComponent ret == NULL\n");
	addition.data = ret;
	addition.hash = HashData32(ret, addition.compSize);
	newAddition.components.push_back(addition);
	newAddition.objHash ^= addition.hash;
}

void SystemSaver::AddNewObject(SystemObj *add)
{
	SaveObj newAddition = {0, {}};
	add->ResetComponentSaveFetching();
	while (add->ComponentFetchingAtEnd() == false)
	{
		AddNewComponentToObject(add, newAddition);
		add->IncrimentComponentFetching();
	}
	objectSaves[add->GetSystemObjectKey()] = newAddition;
}

int SystemSaver::FindFromVector(std::vector<SaveObjData> &components, uint32_t componentKey)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i].componentKey == componentKey)
			return (i);
	}
	return (0);
}

bool SystemSaver::HandleExistingObject(SaveObjData &existing, SystemObj *check, SaveObj &current)
{
	size_t componentSize = check->FetchComponentDataSize();
	void *fetcher = NULL;
	bool allocated = false;
	size_t fetchSize = FETCH_SIZE;
	size_t sizerTool = 0;
	if (componentSize > FETCH_SIZE)
	{
		allocated = true;
		fetchSize = componentSize;
		fetcher = malloc(componentSize);
	}
	else
		fetcher = dataFetcher;
	void *ret = check->FetchComponentSaveData(fetcher, fetchSize, sizerTool);
	uint16_t hash = HashData16(ret, componentSize);
	if (allocated)
		free(fetcher);
	if (existing.hash != hash)
	{
		AddNewComponentToObject(check, current);
		return (true);
	}
	return (false);
}

void SystemSaver::CheckExistingObject(SystemObj *check)
{
	uint32_t key = check->GetSystemObjectKey();
	auto objIndex = objectSaves.find(key);
	SaveObj &current = objIndex->second;
	check->ResetComponentSaveFetching();
	while (check->ComponentFetchingAtEnd() == false)
	{
		uint32_t componentKey = check->FetchComponentUniqueKey();
		int index = FindFromVector(current.components, componentKey);
		if (index == 0)
		{
			AddNewComponentToObject(check, current);
			check->IncrimentComponentFetching();
			continue ;
		}
		SaveObjData &handle = current.components[index];
		if (HandleExistingObject(handle, check, current))
		{
			free(current.components[index].data);
			current.components.erase(current.components.begin() + index);
		}
		check->IncrimentComponentFetching();
	}
}

void SystemSaver::SaveSystemObj(SystemObj *save)
{
	if (save == NULL)
		return ;
	uint32_t key = save->GetSystemObjectKey();
	if (objectSaves.find(key) == objectSaves.end())
		AddNewObject(save);
	else
		CheckExistingObject(save);
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
