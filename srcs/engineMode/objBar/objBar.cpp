
#include "objBar.h"
#include "envHandler.h"
#include "Textures.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "keyboard.h"
#include "mouse.h"
#include "screen.h"
#include "componentRegistry.h"

char buffer[256] = "";

ObjBar::ObjBar()
{
	selectImg = new Image(gameTestTextures.selectedObj.text, {0.0f, 0.0f, 0.6f, 0.6f}, 0.0f, 1000);
	selectImg->drawActive = false;
}

ObjBar::~ObjBar()
{
	delete (selectImg);
}

std::tuple<SystemObj*, SystemObj*> ObjBar::ObjectsSelector(std::unordered_map<uint64_t, SystemObj*> &objs)
{
	std::tuple<SystemObj*, SystemObj*> ret = {NULL, NULL};
	SystemObj *created = NULL;
	ImGui::Begin("Object List");
	if (ImGui::Button("New Object"))
		ret = {NULL, created};
	int iter = 0;
	for (auto &pair : objs)
	{
		SystemObj *obj = pair.second;
		if (self->GetSystemObjectKey() == obj->GetSystemObjectKey())
			continue;

		std::string label = "obj" + std::to_string(iter + 1);
		std::string buttonLabel = "##button" + std::to_string(iter);

		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();

		if (ImGui::Button(("Select" + buttonLabel).c_str()))
			ret = {obj, created};
		iter += 1;
	}
	ImGui::End();
	return (ret);
}

void ObjBar::SelectedWindow()
{
	std::string title = "Current Object";
	if (lastSelected == NULL)
	{
		title = "Not Selected";
		return ;
	}
	ImGui::Begin(title.c_str());

	ImGui::Text("Name:");
	ImGui::SameLine();
	ImGui::Button("Rename");
	ImGui::InputText("n", buffer, IM_ARRAYSIZE(buffer));

	ImGui::Text("Transform:");
	float x = 0.0f, y = 0.0f;
	float w = 0.0f, h = 0.0f;
	float a = 0.0f;
	ImGui::PushItemWidth(73);
	ImGui::InputFloat("x", &x);
	ImGui::SameLine();
	ImGui::InputFloat("y", &y);
	ImGui::InputFloat("w", &w);
	ImGui::SameLine();
	ImGui::InputFloat("h", &h);
	ImGui::InputFloat("a", &a);
	ImGui::PopItemWidth();

	ImGui::Text("Add Component:");
	ImGui::InputText("comp", buffer, IM_ARRAYSIZE(buffer));

	ImGui::Text("Components:");
	for (int i = 0; i < lastSelected->components.size(); i++)
	{
		if (lastSelected->components[i].classType == n_ComponentTypes::TRANSFORM_CLASS)
			continue ;
		ImGui::Text("%s", lastSelected->components[i].type.c_str());
		ImGui::SameLine();
		ImGui::Button("Delete");
	}

	ImGui::End();
}

void ObjBar::ManageSelected()
{
	if (lastSelected == NULL)
		return ;
	if (MouseKeyPressed(n_MouseKeys::MOUSE_RIGHT))
	{
		lastSelected = NULL;
		return ;
	}
	selectImg->drawActive = true;
	t_Point pos = lastSelected->transform->GetPosition();
	selectImg->SetPosition(pos.x, pos.y);
}

void ObjBar::LastUpdate()
{
	if (KeyPressed(SDL_SCANCODE_A))
		self->Destroy();
	selectImg->drawActive = false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	std::unordered_map<uint64_t, SystemObj*> &objs = GetSysEnvData();

	std::tuple<SystemObj*, SystemObj*> ret = ObjectsSelector(objs);
	SystemObj *select = std::get<0>(ret);
	if (select != NULL)
		lastSelected = select;
	ManageSelected();
	SelectedWindow();

	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
