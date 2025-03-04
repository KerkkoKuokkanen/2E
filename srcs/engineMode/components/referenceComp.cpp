
#include "referenceComp.h"

Reference::Reference()
{
	CreateInputField("Name", n_VarType::TEXT, inputName);
	CreateInputField("Key", n_VarType::TEXT, inputKey);
	CreateInputField("Add Referene", n_VarType::BOOL, &added);
}

void Reference::Update()
{
	if (added == false)
		return ;
	std::string name = inputName;
	std::string key = inputKey;
	if (key.length() <= 0)
		return ;
	if (name.length() <= 0)
		return ;
	references[name] = std::stoull(key);
	bzero(inputName, sizeof(char) * 32);
	bzero(inputKey, sizeof(char) * 32);
}
