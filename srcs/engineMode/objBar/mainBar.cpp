
#include "mainBar.h"
#include "imgui.h"
#include "envHandler.h"
#include "roomLoading.h"

bool MainBar::IsHovered()
{
	return (hovered);
}

void MainBar::CreateWindow()
{
	static bool show_popup = false;
	static char name_input[128] = "";

	ImGui::SameLine();

	if (ImGui::Button("Create"))
	{
		show_popup = true;
	}

	if (show_popup)
	{
		ImGui::OpenPopup("Enter Name");

		if (ImGui::BeginPopup("Enter Name"))
		{
			ImGui::Text("Room Name:");

			ImGui::InputText("Name", name_input, sizeof(name_input));

			if (ImGui::Button("Create"))
			{
				std::string createdRoom(name_input);
				CreateNewRoom(createdRoom);
				show_popup = false;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				show_popup = false;
			}

			ImGui::EndPopup();
		}
	}
}

void MainBar::UpdateMainTools()
{
	ImGui::Begin("Engine 2E");
	hovered = ImGui::IsWindowHovered();

	if (ImGui::Button("Play"))
	{
		ChangeEngineMode(false);
		RoomSwitch(GetCurrentRoom());
	}

	ImGui::SameLine();

	if (ImGui::Button("Save"))
		FullSave();

	ImGui::SameLine();

	if (ImGui::Button("Load"))
		ImGui::OpenPopup("LoadPopup");

	if (ImGui::BeginPopup("LoadPopup")) {
		std::vector<std::string> items = GetAllRooms();
		std::string selected = "";
		for (const auto& item : items) {
			if (ImGui::Selectable(item.c_str())) {
				selected = item;
				ImGui::CloseCurrentPopup(); // Optional: close after selection
			}
		}
		ImGui::EndPopup();
		if (selected != "")
		{
			uint16_t room = GetRoomWithName(selected);
			LoadRoom(room);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Room"))
		ImGui::OpenPopup("LoadPopup2");

	if (ImGui::BeginPopup("LoadPopup2")) {
		std::vector<std::string> items = GetAllRooms();
		std::string selected = "";
		for (const auto& item : items) {
			if (ImGui::Selectable(item.c_str())) {
				selected = item;
				ImGui::CloseCurrentPopup(); // Optional: close after selection
			}
		}
		ImGui::EndPopup();
		if (selected != "")
		{
			uint16_t room = GetRoomWithName(selected);
			RoomSwitch(room, {0});
		}
	}

	CreateWindow();

	ImGui::End();
}
