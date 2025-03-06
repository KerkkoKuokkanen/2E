
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

//New Object1, 15824905746164928209
//New Object2, 15824897409774731976

ObjBar::ObjBar()
{
	objSelect = new ObjectSelector();
	objEditor = new ObjectEditor();
}

ObjBar::~ObjBar()
{
	delete objSelect;
	delete objEditor;
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
		NodeData node = {};
		memcpy(node.name, castData + iterator, sizeof(char) * 48);
		iterator += sizeof(char) * 48;
		memcpy(&node.is_folder, castData + iterator, sizeof(bool));
		iterator += sizeof(bool);
		memcpy(&node.objKey, castData + iterator, sizeof(uint64_t));
		iterator += sizeof(uint64_t);
		memcpy(&node.parent_id, castData + iterator, sizeof(int));
		iterator += sizeof(int);
		used.push_back(node);
	}
	objSelect->InitializeObjectSelector(used);
}

void ObjBar::CollectObjectSelector()
{
	ClearSaveData();
	std::vector<NodeData> ret = objSelect->CollectObjSelectorData();
	for (int i = 0; i < ret.size(); i++)
	{
		AddToSave(&ret[i].name, sizeof(char) * 48);
		AddToSave(&ret[i].is_folder, sizeof(bool));
		AddToSave(&ret[i].objKey, sizeof(uint64_t));
		AddToSave(&ret[i].parent_id, sizeof(int));
	}
}

void ObjBar::EngineUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	std::unordered_map<uint64_t, SystemObj*> &objs = GetSysEnvData();

	std::tuple<uint64_t, bool, std::string> ret = objSelect->UpdateObjectSelector(objs, self->GetSystemObjectKey());
	CollectObjectSelector();
	objEditor->UpdateSelected(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
