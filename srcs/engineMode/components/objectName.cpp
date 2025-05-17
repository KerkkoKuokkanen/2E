
#include "objectName.h"

void ObjectName::Init(void *data, size_t size)
{
	if (size <= 1)
		return ;
	std::string result((char*)(data), size);
	name = result;
	AddToSave(data, size);
}

void ObjectName::SetName(std::string name)
{
	ObjectName::name = name;
	ClearSaveData();
	const char *used = name.c_str();
	AddToSave((void*)used, name.length());
}

std::string ObjectName::GetName()
{
	return (name);
}
