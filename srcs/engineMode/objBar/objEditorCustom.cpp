
#include "customComponent.h"
#include "componentRegistry.h"
#include "objEditor.h"
#include "imgui.h"

void ObjectEditor::UpdateCustomComponent(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	t_sysComponent comp = obj->components[compIndex];
	std::string used = comp.type + ".";
	ImGui::Text("%s", used.c_str());
	CustomComponent *cust = (CustomComponent*)comp.obj;
	for (int i = 0; i < cust->inputFields.size(); i++)
	{
		std::string name = std::get<0>(cust->inputFields[i]);
		int varType = std::get<1>(cust->inputFields[i]);
		void *dest = std::get<2>(cust->inputFields[i]);
		switch (varType)
		{
			case n_VarType::INTEGER:
			{
				ImGui::InputInt(name.c_str(), (int*)dest);
				break ;
			}
			case n_VarType::FLOAT:
			{
				ImGui::InputFloat(name.c_str(), (float*)dest);
				break ;
			}
			case n_VarType::TEXT:
			{
				ImGui::InputText(name.c_str(), (char*)dest, sizeof(char) * 32);
				break ;
			}
			case n_VarType::BOOL:
			{
				bool *bol = (bool*)dest;
				if (ImGui::Button(name.c_str()))
					*bol = true;
				else
					*bol = false;
				break ;
			}
			default :
			{
				break ;
			}
		}
	}
}

#include <algorithm>  // For std::transform

void ObjectEditor::ComponentAdder(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Add Component Name");

	static bool showMessage = false;
	static bool goodMessage = false;
	static double startTime = 0.0f;
	const double displayDuration = 2.0;

	static char searchBuffer[64] = {0}; // Input field
	std::vector<std::string> items = GetAllComponentNames();
	static std::vector<std::string> filteredItems; // Stores filtered results
	static bool isDropdownOpen = false;

	// Input field
	if (ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer)))
	{
		std::string searchTerm = searchBuffer;
		filteredItems.clear();

		// Convert search term to lowercase
		std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

		if (!searchTerm.empty())
		{
			isDropdownOpen = true;

			for (const auto& item : items)
			{
				// Convert list item to lowercase for comparison
				std::string lowerItem = item;
				std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(), ::tolower);

				// If the item contains the search term (case insensitive), add it to results
				if (lowerItem.find(searchTerm) != std::string::npos)
				{
					filteredItems.push_back(item); // Store original case version
				}
			}

			// Sort matches by how well they match (closer to start = higher priority)
			std::sort(filteredItems.begin(), filteredItems.end(), [&searchTerm](const std::string& a, const std::string& b) {
				std::string lowerA = a, lowerB = b;
				std::transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
				std::transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
				return lowerA.find(searchTerm) < lowerB.find(searchTerm);
			});
		}
		else
		{
			isDropdownOpen = false;
		}
	}

	// Show dropdown only if matches are found
	if (isDropdownOpen && !filteredItems.empty())
	{
		ImGui::BeginChild("Dropdown", ImVec2(200, 150), true);
		for (const auto& item : filteredItems)
		{
			if (item == "ObjBar")
				continue ;
			if (ImGui::Selectable(item.c_str()))
			{
				strcpy(searchBuffer, item.c_str()); // Set selected item as input
				isDropdownOpen = false; // Close dropdown
			}
		}
		ImGui::EndChild();
	}

	if (ImGui::Button("Add This Component"))
	{
		showMessage = true;
		startTime = ImGui::GetTime();
		std::string compName = searchBuffer;
		void *created = obj->AddComponent(compName);
		if (created == NULL)
			goodMessage = false;
		else
			goodMessage = true;
	}

	if (showMessage)
	{
		double elapsedTime = ImGui::GetTime() - startTime;
		if (elapsedTime < displayDuration)
		{
			if (goodMessage)
				ImGui::Text("Component Created.");
			else
				ImGui::Text("Failed To Create Component.");
		}
		else
			showMessage = false;
	}
}
