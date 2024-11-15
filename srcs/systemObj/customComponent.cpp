
#include "customComponent.h"
#include "componentRegistry.h"

CustomComponent::~CustomComponent()
{
	if (initData != NULL)
		free(initData);
}
