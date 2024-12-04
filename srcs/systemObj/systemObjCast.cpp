
#include "componentRegistry.h"
#include "image.h"
#include "structure.h"
#include "sysEnv.h"
#include "transform.h"

size_t SystemObj::FetchComponentDataSize()
{
	if (componentSaveFetchIndex >= components.size())
		return (0);
	int i = componentSaveFetchIndex;
	switch (components[i].classType)
	{
		case n_ComponentTypes::TRANSFORM_CLASS:
		{
			Transform *trans = (Transform*)components[i].obj;
			return (trans->SaveDataSize());
		}
		case n_ComponentTypes::IMAGE_CLASS:
		{
			Image *img = (Image*)components[i].obj;
			return (img->GetSaveDataSize());
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			Structure *structure = (Structure*)components[i].obj;
			return (structure->GetSaveDataSize());
		}
		default:
		{
			CustomComponent *cust = (CustomComponent*)components[i].obj;
			return (cust->GetComponentSize());
		}
	}
	return (0);
}

void *SystemObj::FetchComponentSaveData(void *buffer, size_t bufferSize, size_t &compSize)
{
	if (componentSaveFetchIndex >= components.size())
		return (NULL);
	int i = componentSaveFetchIndex;
	switch (components[i].classType)
	{
		case n_ComponentTypes::TRANSFORM_CLASS:
		{
			Transform *trans = (Transform*)components[i].obj;
			void *ret = trans->CollectSaveData(buffer, bufferSize, compSize);
			if (ret == NULL)
				return (NULL);
			return (buffer);
		}
		case n_ComponentTypes::IMAGE_CLASS:
		{
			Image *img = (Image*)components[i].obj;
			void *ret = img->CollectSaveData(buffer, bufferSize, compSize);
			if (ret == NULL)
				return (NULL);
			return (buffer);
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			Structure *structure = (Structure*)components[i].obj;
			void *ret = structure->CollectSaveData(buffer, bufferSize, compSize);
			if (ret == NULL)
				return (NULL);
			return (buffer);
		}
		default:
		{
			CustomComponent *cust = (CustomComponent*)components[i].obj;
			return (cust->CollectSaveData(compSize));
		}
	}
	return (NULL);
}

void SystemObj::UpdateSystemObj()
{
	for (int i = 0; i < components.size(); i++)
	{
		switch (components[i].classType)
		{
			case n_ComponentTypes::TRANSFORM_CLASS:
				break ;
			case n_ComponentTypes::IMAGE_CLASS:
				break ;
			case n_ComponentTypes::STRUCTURE_CLASS:
				break ;
			default :
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				if (components[i].started == false)
				{
					cust->Start();
					components[i].started = true;
				}
				cust->Update();
				break ;
			}
		}
	}
}

void SystemObj::GiveComponentId(void *component, uint32_t classType, uint32_t id)
{
	if (classType == n_ComponentTypes::TRANSFORM_CLASS)
		return ;
	else if (classType == n_ComponentTypes::IMAGE_CLASS)
	{
		Image *obj = (Image*)component;
		obj->self = this;
		obj->id = id;
		Transform *trans = (Transform*)components[components.size() - 1].obj;
		trans->AddImage(obj);
		return ;
	}
	else if (classType == n_ComponentTypes::STRUCTURE_CLASS)
	{
		Structure *obj = (Structure*)component;
		obj->self = this;
		obj->id = id;
		Transform *trans = (Transform*)components[components.size() - 1].obj;
		trans->AddStructure(obj);
		return ;
	}
	CustomComponent *obj = (CustomComponent*)component;
	obj->ownId = id;
}

void SystemObj::DeleteComponentOwn(void *component, uint32_t classType)
{
	switch (classType)
	{
		case n_ComponentTypes::TRANSFORM_CLASS:
			return ;
		case n_ComponentTypes::IMAGE_CLASS:
		{
			Image *img = (Image*)component;
			Transform *trans = (Transform*)components[components.size() - 1].obj;
			trans->RemoveImage(img);
			delete img;
			break ;
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			Structure *structure = (Structure*)component;
			Transform *trans = (Transform*)components[components.size() - 1].obj;
			trans->RemoveStructure(structure);
			delete structure;
			break ;
		}
		default :
		{
			CustomComponent *cust = (CustomComponent*)component;
			cust->Destroy();
			delete cust;
			break ;
		}
	}
}

void SystemObj::AddNewTransformComponent(Transform *transf)
{
	Transform *trans = (Transform*)components[components.size() - 1].obj;
	delete trans;
	components.erase(components.begin() + components.size() - 1);
	transform = transf;
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i].classType == n_ComponentTypes::IMAGE_CLASS)
			transf->AddImage((Image*)components[i].obj);
		else if (components[i].classType == n_ComponentTypes::STRUCTURE_CLASS)
			transf->AddStructure((Structure*)components[i].obj);
	}
	t_sysComponent add = {0, n_ComponentTypes::TRANSFORM_CLASS, true, "", transf};
	components.push_back(add);
}

/* SystemObj is not aware if components get
deleted outside this function */
SystemObj::~SystemObj()
{
	deleting = true;
	for (int i = 0; i < components.size(); i++)
		DeleteComponentOwn(components[i].obj, components[i].classType);
	if (controller == NULL)
		return ;
	Transform *trans = (Transform*)components[components.size() - 1].obj;
	delete trans;
	SysEnv *env = (SysEnv*)controller;
	env->RemoveObject(this);
}
