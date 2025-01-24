
#ifndef ENGINE_TOOL_H
# define ENGINE_TOOL_H

# include "customComponent.h"
# include "componentRegistry.h"

class EngineTool : public CustomComponent
{
	public:
		bool objectSelected = false;
};

REGISTER_COMPONENT(EngineTool);

#endif
