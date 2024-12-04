
#include "sysSaver.h"
#include "commonTools.h"

void SystemSaver::AddNewComponentToObject(SystemObj *add, SaveObj &newAddition)
{
	changeSpotted = true;
	SaveObjData addition;
	size_t sizerTool = 0;
	addition.compSize = add->FetchComponentDataSize();
	if (addition.compSize == 0)
		return ;
	addition.componentKey = add->FetchComponentUniqueKey();
	addition.componentType = add->FetchComponentClassType();
	addition.data = malloc(addition.compSize);
	void *ret = add->FetchComponentSaveData(addition.data, addition.compSize, sizerTool);
	if (ret == NULL)
		printf("SystemSaver.cpp: AddNewComponent ret == NULL\n");
	if (addition.componentType >= 999) //The component type starting number for custom components
		memcpy(addition.data, ret, addition.compSize);
	else
		addition.data = ret;
	addition.hash = HashData32(ret, addition.compSize);
	newAddition.components.push_back(addition);
	newAddition.objHash ^= addition.hash;
}

void SystemSaver::AddNewObject(SystemObj *add)
{
	SaveObj newAddition = {0, add->saveable, {}};
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
	return (-1);
}

bool SystemSaver::HandleExistingObject(SaveObjData &existing, SystemObj *check, SaveObj &current)
{
	void *fetcher = dataFetcher;
	size_t fetchSize = FETCH_SIZE;
	size_t sizerTool = 0;
	void *ret = check->FetchComponentSaveData(fetcher, fetchSize, sizerTool);
	if (ret == NULL)
		printf("sysSaver: dataFetcher was too small, custom components can not be the issue\n");
	uint32_t hash = HashData32(ret, sizerTool);
	if (existing.hash != hash)
	{
		AddNewComponentToObject(check, current);
		return (true);
	}
	return (false);
}

void SystemSaver::CheckExistingObject(SystemObj *check)
{
	uint64_t key = check->GetSystemObjectKey();
	auto objIndex = objectSaves.find(key);
	SaveObj &current = objIndex->second;
	check->ResetComponentSaveFetching();
	while (check->ComponentFetchingAtEnd() == false)
	{
		uint32_t componentKey = check->FetchComponentUniqueKey();
		int index = FindFromVector(current.components, componentKey);
		if (index == -1)
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
	uint64_t key = save->GetSystemObjectKey();
	if (objectSaves.find(key) == objectSaves.end())
		AddNewObject(save);
	else
		CheckExistingObject(save);
}

void SystemSaver::RemoveObjectFromSaver(SystemObj *obj)
{
	uint64_t key = obj->GetSystemObjectKey();
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

void SystemSaver::SetSnapObjects(std::vector<SnapObject> &setted, SaveObj &current, size_t &totalSize, uint64_t key)
{
	SnapObject addition = {0, current.objHash, key, (uint8_t)current.saveable, {}};
	for (int i = 0; i < current.components.size(); i++)
	{
		addition.snapObj.push_back(&current.components[i]);
		addition.objSize += current.components[i].compSize;
		addition.objSize += sizeof(uint32_t);
		addition.objSize += sizeof(uint32_t);
		totalSize += current.components[i].compSize;
		totalSize += sizeof(uint32_t);
		totalSize += sizeof(uint32_t);
	}
	setted.push_back(addition);
}

void SystemSaver::SetToSnapData(uint8_t *snap, std::vector<SnapObject> &saveObjs)
{
	size_t offset = 0;
	for (int i = 0; i < saveObjs.size(); i++)
	{
		uint32_t saveObjSize = (uint32_t)saveObjs[i].objSize;
		memcpy(snap + offset, &saveObjs[i].objKey, sizeof(uint64_t)); offset += sizeof(uint64_t);
		memcpy(snap + offset, &saveObjs[i].objHash, sizeof(uint32_t)); offset += sizeof(uint32_t);
		memcpy(snap + offset, &saveObjs[i].saveable, sizeof(uint8_t)); offset += sizeof(uint8_t);
		memcpy(snap + offset, &saveObjSize, sizeof(uint32_t)); offset += sizeof(uint32_t);
		for (int j = 0; j < saveObjs[i].snapObj.size(); j++)
		{
			SaveObjData *sod = saveObjs[i].snapObj[j];
			uint32_t compSize = (uint32_t)sod->compSize;
			memcpy(snap + offset, &sod->componentType, sizeof(uint32_t)); offset += sizeof(uint32_t);
			memcpy(snap + offset, &compSize, sizeof(uint32_t)); offset += sizeof(uint32_t);
			memcpy(snap + offset, sod->data, (size_t)sod->compSize); offset += (size_t)sod->compSize;
		}
	}
}

void SystemSaver::ClearSnapshotsFront()
{
	if (currentSnapIndex == snapShots.size() - 1)
		return ;
	for (int i = currentSnapIndex + 1; i < snapShots.size();)
	{
		if (snapShots[i].data != NULL)
			free(snapShots[i].data);
		snapShots.erase(snapShots.begin() + i);
	}
}

void SystemSaver::TakeSnapShot()
{
	ClearSnapshotsFront();
	changeSpotted = false;
	size_t totalSize = 0;
	std::vector<SnapObject> saveObjs;
	for (auto &[key, obj] : objectSaves)
		SetSnapObjects(saveObjs, obj, totalSize, key);
	size_t newSize = totalSize + (sizeof(uint32_t) * saveObjs.size() * 2)
								+ (sizeof(uint8_t) * saveObjs.size())
								+ (sizeof(uint64_t) * saveObjs.size());
	void *snap = malloc(newSize);
	SetToSnapData((uint8_t*)snap, saveObjs);
	uint64_t hash = HashData64(snap, newSize);
	snapShots.push_back((SnapShot){hash, (uint32_t)newSize, snap});
	if (snapShots.size() > SNAPSHOT_AMOUNT)
	{
		if (snapShots[0].data != NULL)
			free(snapShots[0].data);
		snapShots.erase(snapShots.begin() + 0);
	}
	currentSnapIndex = snapShots.size() - 1;
}

void SystemSaver::SetSaveFile(const std::string file)
{
	saveFile = file;
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
	for (int i = 0; i < snapShots.size(); i++)
	{
		if (snapShots[i].data != NULL)
			free(snapShots[i].data);
	}
}
