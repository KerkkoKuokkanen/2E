
#include "customComponent.h"
#include "objEditor.h"
#include "imgui.h"

void ObjectEditor::UpdateCustomComponent(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	t_sysComponent comp = obj->components[compIndex];
	std::string used = comp.type + ".";
	ImGui::Text("%s", used.c_str());
	CustomComponent *cust = (CustomComponent*)comp.obj;
	for (int i = 0; i < cust->inputFields.size(); i++)
	{
		std::string name = std::get<0>(cust->inputFields[i]);
		int varType = std::get<1>(cust->inputFields[i]);
		void *dest = std::get<2>(cust->inputFields[i]);
		switch (varType)
		{
			case n_VarType::INTEGER:
			{
				ImGui::InputInt(name.c_str(), (int*)dest);
				break ;
			}
			case n_VarType::FLOAT:
			{
				ImGui::InputFloat(name.c_str(), (float*)dest);
				break ;
			}
			case n_VarType::TEXT:
			{
				ImGui::InputText(name.c_str(), (char*)dest, sizeof(char) * 32);
				break ;
			}
			case n_VarType::BOOL:
			{
				bool *bol = (bool*)dest;
				if (ImGui::Button(name.c_str()))
					*bol = true;
				else
					*bol = false;
				break ;
			}
			default :
			{
				break ;
			}
		}
	}
}
