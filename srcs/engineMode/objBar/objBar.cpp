
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

void ObjBar::Init(void *data, size_t size)
{
	if (size < sizeof(NodeData))
		return ;
	size_t iterator = 0;
	char *castData = (char*)data;
	std::vector<NodeData> used;
	while (iterator < size)
	{
		NodeData *node = (NodeData*)castData + iterator;
		iterator += sizeof(NodeData);
		used.push_back(*node);
	}
	objSelect->InitializeObjectSelector(used);
}

void ObjBar::CollectObjectSelector()
{
	std::vector<NodeData> ret = objSelect->CollectObjSelectorData();
	ClearSaveData();
	for (int i = 0; i < ret.size(); i++)
		AddToSave(&ret[i], sizeof(NodeData));
}

void ObjBar::LastUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	std::unordered_map<uint64_t, SystemObj*> &objs = GetSysEnvData();

	objSelect->UpdateObjectSelector(objs, self->GetSystemObjectKey());
	CollectObjectSelector();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
