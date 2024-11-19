
#include "componentRegistry.h"
#include "image.h"
#include "structure.h"

void *SystemObj::FetchComponentSaveData(void *buffer, size_t bufferSize, size_t &compSize)
{
	if (componentSaveFetchIndex >= components.size())
		return (NULL);
	int i = componentSaveFetchIndex;
	componentSaveFetchIndex += 1;
	switch (components[i].classType)
	{
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
			case n_ComponentTypes::IMAGE_CLASS:
				break ;
			case n_ComponentTypes::STRUCTURE_CLASS:
				break ;
			default :
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				cust->Update();
				break ;
			}
		}
	}
}

/* SystemObj is not aware if components get
deleted outside this function */
SystemObj::~SystemObj()
{
	for (int i = 0; i < components.size(); i++)
	{
		switch (components[i].classType)
		{
			case n_ComponentTypes::IMAGE_CLASS:
			{
				Image *img = (Image*)components[i].obj;
				delete img;
				break ;
			}
			case n_ComponentTypes::STRUCTURE_CLASS:
			{
				Structure *structure = (Structure*)components[i].obj;
				delete structure;
				break ;
			}
			default :
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				cust->Destroy();
				delete cust;
				break ;
			}
		}
	}
}
