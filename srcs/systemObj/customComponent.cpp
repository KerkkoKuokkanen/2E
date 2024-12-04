
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
}

void CustomComponent::ResetMemory()
{
	if (initData != NULL && initDataSize != 0)
	{
		if (usingCustomPool)
			CustomMemoryFree(initData);
		else
			free(initData);
	}
}

void *CustomComponent::CollectSaveData(size_t &size)
{
	ResetMemory();
	usingCustomPool = false;
	for (int i = 0; i < saveTracking.size(); i++)
	{
		tracking add = saveTracking[i];
		void *data = std::get<0>(add);
		size_t size = std::get<1>(add);
		AddToSaveData(data, size);
	}
	size = initDataSize;
	return (initData);
}

CustomComponent::~CustomComponent()
{
	if (self != NULL)
		self->RemoveComponent(ownId);
	ResetMemory();
}

void CustomComponent::InitializeMemory()
{
	initData = CustomMemoryAlloc();
	usingCustomPool = true;
	if (initData == NULL)
	{
		initData = malloc(DEFAULT_SAVE_SIZE);
		usingCustomPool = false;
	}
	totalSize = DEFAULT_SAVE_SIZE;
}

void CustomComponent::ReallocateMemory(size_t addSize)
{
	if (usingCustomPool)
	{
		CustomMemoryFree(initData);
		usingCustomPool = false;
		initData = NULL;
	}
	initData = realloc(initData, addSize + totalSize);
	totalSize = addSize + totalSize;
}

void CustomComponent::AddToSaveData(void *addition, size_t addSize)
{
	if (totalSize == 0)
		InitializeMemory();
	if (addSize > (totalSize - initDataSize))
		ReallocateMemory(addSize);
	char *byteData = (char*)initData;
	memcpy(byteData + initDataSize, addition, addSize);
	initDataSize += addSize;
}
