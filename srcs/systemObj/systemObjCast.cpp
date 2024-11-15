
#include "componentRegistry.h"
#include "image.h"
#include "structure.h"

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
			case n_ComponentTypes::CUSTOM_CLASS:
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				cust->Destroy();
				delete cust;
				break ;
			}
		}
	}
}
