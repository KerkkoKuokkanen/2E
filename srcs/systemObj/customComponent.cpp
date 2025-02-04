
#include "customComponent.h"
#include "memoryPool.h"

static MemoryPool &GetCustomMemPool()
{
	static MemoryPool memPool(MB_SIZE);
	return (memPool);
}

static void *CustomMemoryAlloc()
{
	MemoryPool &alloc = GetCustomMemPool();
	void *retPool = alloc.Allocate(DEFAULT_SAVE_SIZE);
	return (retPool);
}

static void CustomMemoryFree(void *ptr)
{
	MemoryPool &alloc = GetCustomMemPool();
	alloc.Free(ptr);
}

void CustomComponent::AddToSave(void *addition, size_t addSize)
{
	tracking add(addition, addSize);
	saveTracking.push_back(add);
	initDataSize += addSize;
}

void *CustomComponent::CollectSaveData(size_t &size)
{
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

CustomComponent::~CustomComponent()
{
	if (self != NULL)
		self->RemoveComponent(ownId);
}
