
#include "componentRegistry.h"
#include "customComponent.h"
#include <unordered_map>

std::unordered_map<std::string, ComponentFactory>& GetComponentRegistry() {
	static std::unordered_map<std::string, ComponentFactory> componentFactories;
	return componentFactories;
}

void RegisterComponent(const std::string& type, ComponentFactory factory)
{
	auto& registry = GetComponentRegistry();
	if (registry.find(type) == registry.end())
		registry[type] = factory;
}

CustomComponent *CreateComponent(const std::string& type)
{
	auto& registry = GetComponentRegistry();
	auto it = registry.find(type);
	if (it != registry.end())
		return (it->second());
	return (NULL);
}
