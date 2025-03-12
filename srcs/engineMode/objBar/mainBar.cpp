
#include "mainBar.h"
#include "imgui.h"
#include "envHandler.h"
#include "camera.h"

bool MainBar::IsHovered()
{
	return (hovered);
}

void MainBar::UpdateMainTools()
{
	ImGui::Begin("Engine 2E");
	hovered = ImGui::IsWindowHovered();

	if (ImGui::Button(" X "))
	{
		SystemObj *obj = FindSystemObject(3851118115171993389);
		if (obj != NULL)
		{
			Camera *cam = (Camera*)obj->GetComponent("Camera");
			if (cam != NULL)
			{
				cam->SetCameraPosition(0.0f, 0.0f);
				cam->SetCameraZoom(10.0f);
			}
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Play"))
		printf("play\n");

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
		printf("stop\n");

	ImGui::SameLine();

	if (ImGui::Button("Save"))
		SaveEngineRoom();

	ImGui::SameLine();

	if (ImGui::Button("Load"))
		printf("load");

	ImGui::SameLine();

	if (ImGui::Button("Room"))
		printf("room\n");

	ImGui::End();
}
