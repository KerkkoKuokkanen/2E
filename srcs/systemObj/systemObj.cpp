
#include <random>
#include "componentRegistry.h"
#include "sysEnv.h"

static bool SortComponents(const t_sysComponent& a, const t_sysComponent& b)
{
	return a.classType > b.classType;
}

static uint64_t GenerateRandomNumber()
{
	static std::mt19937_64 gen(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> distrib(0, (1ULL << 42) - 1);
	return distrib(gen);
}

static uint32_t GetUniqueKeyForSysComponent()
{
	static uint32_t key = rand() % 4294967293;
	key += 1;
	if (key >= 4294967293)
		key = 0;
	return (key);
}

static uint64_t GetUniqueKeyForSysObj()
{
	static uint64_t key = rand() % 4194304;
	uint64_t randomPart = GenerateRandomNumber();
	key += 1;
	if (key >= 4194304)
		key = 0;
	uint64_t ret = (key << 42) | randomPart;
	return (ret);
}

void SystemObj::AddObjectController(void *controller)
{
	if (controller == NULL)
		return ;
	SystemObj::controller = controller;
	SysEnv *env = (SysEnv*)controller;
	env->AddObject(this);
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

void SystemObj::AddComponent(const std::string component, void *initData, size_t initDataSize)
{
	t_sysComponent add;
	add.uniqueKey = GetUniqueKeyForSysComponent();
	add.obj = (void*)CreateComponent(component);
	CustomComponent *comp = (CustomComponent*)add.obj;
	comp->self = this;
	comp->Init(initData, initDataSize);
	add.type = component;
	add.classType = GetComponentKeyWithName(component);
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void SystemObj::AddComponent(const std::string component)
{
	t_sysComponent add;
	add.uniqueKey = GetUniqueKeyForSysComponent();
	add.obj = (void*)CreateComponent(component);
	if (add.obj == NULL)
		return ;
	CustomComponent *comp = (CustomComponent*)add.obj;
	comp->self = this;
	comp->Init(NULL, 0);
	add.type = component;
	add.classType = GetComponentKeyWithName(component);
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void SystemObj::AddComponent(void *component, uint32_t classType, const std::string name)
{
	t_sysComponent add = {GetUniqueKeyForSysComponent(), classType, name, component};
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void SystemObj::AddComponent(void *component, const std::string name)
{
	uint32_t classType = GetComponentKeyWithName(name);
	if (name == IMAGE_COMPONENT)
		classType = n_ComponentTypes::IMAGE_CLASS;
	else if (name == STRUCTURE_COMPONENT)
		classType = n_ComponentTypes::STRUCTURE_CLASS;
	t_sysComponent add = {GetUniqueKeyForSysComponent(), classType, name, component};
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
}

void SystemObj::AddComponent(void *component, uint32_t classType)
{
	const std::string name = GetComponentNameWithKey(classType);
	t_sysComponent add = {GetUniqueKeyForSysComponent(), classType, name, component};
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
