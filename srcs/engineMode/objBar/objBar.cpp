
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

ObjBar::ObjBar()
{
	objSelect = new ObjectSelector();
}

ObjBar::~ObjBar()
{
	delete objSelect;
}

void ObjBar::LastUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	std::unordered_map<uint64_t, SystemObj*> &objs = GetSysEnvData();

	objSelect->UpdateObjectSelector(objs, self->GetSystemObjectKey());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
