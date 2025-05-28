
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
#include "saveInterface.h"
#include "roomLoading.h"


//New Object1, 15824905746164928209
//New Object2, 15824897409774731976

ObjBar::ObjBar()
{
	objSelect = new ObjectSelector();
	objEditor = new ObjectEditor();
	mainBar = new MainBar();
	img = new Image("saving", {9.0f, -9.0f, 1.2f, 1.2f}, 0.0f, 0);
	img->SetTransformType(n_TransformTypes::NO_TRANSFORM);
	img->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
}

ObjBar::~ObjBar()
{
	delete objSelect;
	delete objEditor;
	delete mainBar;
	delete img;
}

bool ObjBar::HoveredOverWindow()
{
	bool selec = GetObjSelectorHover();
	bool elect = false;
	bool edit = false;
	if (objEditor != NULL)
		edit = objEditor->IsHovered();
	if (mainBar != NULL)
		elect = mainBar->IsHovered();
	if (selec || edit || elect)
		return (true);
	return (false);
}

void ObjBar::ImgUpdate()
{
	angle -= 0.18f;
	img->SetAngle(angle);
	if (fabs(angle) > 1000000.0f)
		angle = 0.0f;
	if (GetSaving() == false)
	{
		img->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
		return ;
	}
	img->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ObjBar::InitSecondaryHierarchy(void *hier)
{
	objSelect->InitSecondaryHierarchy(hier);
}

void ObjBar::EngineUpdate()
{
	ImgUpdate();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	std::unordered_map<uint64_t, SystemObj*> &objs = GetSysEnvData();

	std::tuple<uint64_t, bool, std::string> ret = objSelect->UpdateObjectSelector(objs, self->GetSystemObjectKey());
	objEditor->UpdateSelected(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
	mainBar->UpdateMainTools();
	ChangeOverImgui(HoveredOverWindow());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
