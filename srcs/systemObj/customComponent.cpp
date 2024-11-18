
#include "customComponent.h"

CustomComponent::~CustomComponent()
{
	if (initData != NULL)
		free(initData);
}
