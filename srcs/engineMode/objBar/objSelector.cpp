
#include "objSelector.h"
#include "imgui.h"

void ObjectSelector::UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self)
{
	std::tuple<SystemObj*, SystemObj*> ret = {NULL, NULL};
	SystemObj *created = NULL;
	ImGui::Begin("Object List");
	if (ImGui::Button("New Object"))
	{
		created = new SystemObj();
		ret = {NULL, created};
	}
	int iter = 0;
	for (auto &pair : objs)
	{
		SystemObj *obj = pair.second;
		if (self == obj->GetSystemObjectKey())
			continue ;

		std::string label = "obj" + std::to_string(iter + 1);
		std::string buttonLabel = "##button" + std::to_string(iter);

		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		if (ImGui::Button(("Select" + buttonLabel).c_str()))
			ret = {obj, created};

		iter += 1;
	}
	ImGui::End();
}
