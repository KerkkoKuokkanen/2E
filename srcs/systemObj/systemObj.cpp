
#include "componentRegistry.h"
#include "sysEnv.h"

static bool SortComponents(const t_sysComponent& a, const t_sysComponent& b)
{
	return a.classType > b.classType;
}

static uint32_t GetUniqueKeyForSysObj()
{
	static uint32_t key = 0;
	key += 1;
	if (key >= 4294967290)
		key = 0;
	return (key);
}

static uint32_t GetUniqueKeyForSysComponent()
{
	static uint32_t key = 0;
	key += 1;
	if (key >= 4294967290)
		key = 0;
	return (key);
}

SystemObj::SystemObj(void *sysEnv)
{
	uniqueSystemObjKey = GetUniqueKeyForSysObj();
	controller = sysEnv;
	if (controller == NULL)
		return ;
	SysEnv *env = (SysEnv*)controller;
	env->AddObject(this);
}

void SystemObj::AddComponentCustom(const std::string component, void *initData)
{
	t_sysComponent add;
	add.uniqueKey = GetUniqueKeyForSysObj();
	add.obj = (void*)CreateComponent(component);
	CustomComponent *comp = (CustomComponent*)add.obj;
	comp->self = this;
	comp->Init(initData);
	add.type = component;
	add.classType = GetComponentKeyWithName(component);
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void SystemObj::AddComponentStruct(void *component, uint32_t classType, const std::string name)
{
	t_sysComponent add = {GetUniqueKeyForSysObj(), classType, name, component};
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void *SystemObj::GetComponent(const std::string &component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (component == components[i].type)
			return (components[i].obj);
	}
	return (NULL);
}

std::vector<void*> SystemObj::GetComponents(const std::string &component)
{
	std::vector<void*> ret = {};
	for (int i = 0; i < components.size(); i++)
	{
		if (component == components[i].type)
			ret.push_back(components[i].obj);
	}
	return (ret);
}

bool SystemObj::ComponentFetchingAtEnd()
{
	if (saveable == false)
		return (false);
	if (componentSaveFetchIndex >= components.size())
		return (true);
	return (false);
}

uint32_t SystemObj::FetchComponentClassType()
{
	if (componentSaveFetchIndex >= components.size())
		return (n_ComponentTypes::NO_CLASS);
	return (components[componentSaveFetchIndex].classType);
}

uint32_t SystemObj::FetchComponentUniqueKey()
{
	if (componentSaveFetchIndex >= components.size())
		return (0);
	return (components[componentSaveFetchIndex].uniqueKey);
}
