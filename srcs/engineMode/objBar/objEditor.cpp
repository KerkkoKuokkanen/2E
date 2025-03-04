
#include "objEditor.h"
#include "imgui.h"
#include "transform.h"
#include "customComponent.h"

void ObjectEditor::TransformUpdate(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Transform");
	Transform *trans = (Transform*)obj->GetComponent("transform");
	t_Point pos = trans->GetPosition();
	float w = trans->GetWidth();
	float h = trans->GetHeight();
	float a = trans->GetAngle();

	ImGui::Text("Position:");
	ImGui::InputFloat("X", &pos.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Y", &pos.y, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Size:");
	ImGui::InputFloat("Width", &w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Height", &h, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Rotation:");
	ImGui::InputFloat("Angle", &a, 0.01f, 1.0f, "%.2f");

	trans->Position(pos.x, pos.y);
	trans->Angle(a);
	trans->Width(w);
	trans->Height(h);
}

void ObjectEditor::ComponentSelector(SystemObj *obj)
{
	ImGui::Text("Components:");
	for (int i = 0; i < obj->components.size(); i++)
	{
		t_sysComponent comp = obj->components[i];
		std::string str = comp.type + " " + std::to_string(i);
		if (ImGui::Button(str.c_str()))
		{
			selectWindow = comp.classType;
			compIndex = i;
		}
	}
}

void ObjectEditor::UpdateSelectedWindow(SystemObj *obj)
{
	switch (selectWindow)
	{
		case 0:
			ComponentSelector(obj);
			break ;
		case n_ComponentTypes::TRANSFORM_CLASS:
			TransformUpdate(obj);
			break ;
		default:
			UpdateCustomComponent(obj);
			break ;
	}
}

void ObjectEditor::UpdateSelected(uint64_t key, bool selected, std::string name)
{
	ImGui::Begin("Object Editor");
	if (selected == false)
	{
		ImGui::Text("Object Not Selected");
		ImGui::End();
		return ;
	}
	SystemObj *obj = FindSystemObject(key);
	name += ":";
	ImGui::Text("%s", name.c_str());
	UpdateSelectedWindow(obj);
	ImGui::End();
}
