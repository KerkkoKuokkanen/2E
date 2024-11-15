
#include "componentRegistry.h"

static bool SortComponents(const t_sysComponent& a, const t_sysComponent& b)
{
	return a.classType < b.classType;
}

static unsigned int GetUniqueKeyForSysObj()
{
	static unsigned int key = 0;
	key += 1;
	if (key >= 4294967290)
		key = 0;
	return (key);
}

SystemObj::SystemObj()
{
	uniqueSystemObjKey = GetUniqueKeyForSysObj();
}

void SystemObj::AddComponentCustom(const char *component, void *initData)
{
	t_sysComponent add;
	add.obj = (void*)CreateComponent(component);
	CustomComponent *comp = (CustomComponent*)add.obj;
	comp->self = this;
	comp->Init(initData);
	add.type = component;
	add.classType = n_ComponentTypes::CUSTOM_CLASS;
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void SystemObj::AddComponentStruct(void *component, int classType, const char *name)
{
	t_sysComponent add = {classType, name, component};
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void *SystemObj::GetComponent(const char *component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (strcmp(component, components[i].type))
			return (components[i].obj);
	}
	return (NULL);
}

std::vector<void*> SystemObj::GetComponents(const char *component)
{
	std::vector<void*> ret = {};
	for (int i = 0; i < components.size(); i++)
	{
		if (strcmp(component, components[i].type))
			ret.push_back(components[i].obj);
	}
	return (ret);
}
