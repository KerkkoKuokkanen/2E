
#include "componentRegistry.h"
#include "image.h"
#include "structure.h"

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

void SystemObj::AddComponent(const char *component)
{
	t_sysComponent add;
	add.obj = (void*)CreateComponent(component);
	add.type = component;
	add.classType = n_ComponentTypes::CUSTOM_CLASS;
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

void SystemObj::UpdateSystemObj()
{
	for (int i = 0; i < components.size(); i++)
	{
		switch (components[i].classType)
		{
			case n_ComponentTypes::IMAGE_CLASS:
			{
				Image *img = (Image*)components[i].obj;
				img->Draw();
				break ;
			}
			case n_ComponentTypes::STRUCTURE_CLASS:
			{
				Structure *structure = (Structure*)components[i].obj;
				structure->Draw();
				break ;
			}
			case n_ComponentTypes::CUSTOM_CLASS:
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				cust->Update();
				break ;
			}
		}
	}
}
