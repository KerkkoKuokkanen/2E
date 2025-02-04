
#include "objBar.h"
#include "envHandler.h"
#include "Textures.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "screen.h"

ObjBar::ObjBar()
{
	selectImg = new Image(gameTestTextures.selectedObj.text, {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 1000);
	selectImg->drawActive = false;
}

void ObjBar::Init(void *data, size_t size)
{
	AddToSave(&save, sizeof(bool));
}

ObjBar::~ObjBar()
{
	delete (selectImg);
}

void ObjBar::ObjectsSelector(std::unordered_map<uint64_t, SystemObj*> &objs)
{
	ImGui::Begin("Object List");
	int iter = 0;
	for (auto &pair : objs)
	{
		SystemObj *obj = pair.second;
		std::string label = "obj" + std::to_string(iter + 1);
		ImGui::Text("%s", label.c_str());

		/* if (ImGui::Button(("Select " + label).c_str()))
			std::cout << label << " selected!" << std::endl; */
		iter += 1;
	}
	ImGui::End();
}

void ObjBar::Update()
{
	selectImg->drawActive = false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	std::unordered_map<uint64_t, SystemObj*> &objs = GetSysEnvData();

	ObjectsSelector(objs);

	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
