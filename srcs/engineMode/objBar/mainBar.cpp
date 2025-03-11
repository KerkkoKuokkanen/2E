
#include "mainBar.h"
#include "imgui.h"
#include "envHandler.h"

bool MainBar::IsHovered()
{
	return (hovered);
}

void MainBar::UpdateMainTools()
{
	ImGui::Begin("Engine 2E");
	hovered = ImGui::IsWindowHovered();

	if (ImGui::Button(" X "))
		printf("center\n");

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
