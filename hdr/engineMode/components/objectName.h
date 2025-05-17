
#ifndef OBJECT_NAME_H
# define OBJECT_NAME_H

#include "customComponent.h"
#include "componentRegistry.h"
#include "string.h"

class ObjectName : public CustomComponent
{
	private:
		std::string name = "random_object";
	public:
		void Init(void *data, size_t size) override;
		void SetName(std::string name);
		std::string GetName();
		
};

REGISTER_COMPONENT(ObjectName);

#endif
