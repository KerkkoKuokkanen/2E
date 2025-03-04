
#include "customComponent.h"
#include "memoryPool.h"
#include "commonTools.h"

void CustomComponent::ClearSaveData()
{
	for (int i = 0; i < saveTracking.size(); i++)
	{
		void *data = std::get<0>(saveTracking[i]);
		if (data != NULL)
			free(data);
	}
	saveTracking.clear();
	initDataSize = 0;
}

void CustomComponent::RemoveFromSave(void *removed, size_t size)
{
	uint64_t hash = HashData64(removed, size);
	for (int i = 0; i < saveTracking.size(); i++)
	{
		uint64_t check = std::get<2>(saveTracking[i]);
		if (hash == check)
		{
			void *data = std::get<0>(saveTracking[i]);
			if (data != NULL)
				free(data);
			initDataSize -= size;
			saveTracking.erase(saveTracking.begin() + i);
			break ;
		}
	}
}

void CustomComponent::AddToSave(void *addition, size_t addSize)
{
	void *copy = malloc(addSize);
	memcpy(copy, addition, addSize);
	uint64_t hash = HashData64(addition, addSize);
	tracking add(copy, addSize, hash);
	saveTracking.push_back(add);
	initDataSize += addSize;
}

size_t CustomComponent::GetComponentSize()
{
	if (initDataSize == 0)
	{
		if (self == NULL)
			return (0);
		return (sizeof(bool));
	}
	return (initDataSize);
}

void *CustomComponent::CollectSaveData(size_t &size)
{
	if (initDataSize == 0)
	{
		bool use = true;
		void *saveData = malloc(sizeof(bool));
		char *cast = (char*)saveData;
		size = sizeof(bool);
		memcpy(cast, &use, sizeof(bool));
		return (saveData);
	}
	size_t offset = 0;
	size = initDataSize;
	void *saveData = malloc(initDataSize);
	char *cast = (char*)saveData;
	for (int i = 0; i < saveTracking.size(); i++)
	{
		void *data = std::get<0>(saveTracking[i]);
		size_t dataSize = std::get<1>(saveTracking[i]);
		memcpy(cast + offset, data, dataSize);
		offset += dataSize;
	}
	return (saveData);
}

void CustomComponent::CreateInputField(std::string name, int varType, void *dest)
{
	std::tuple<std::string, int, void*> add = {name, varType, dest};
	inputFields.push_back(add);
}

CustomComponent::~CustomComponent()
{
	ClearSaveData();
	if (self != NULL)
		self->RemoveComponent(ownId);
}
