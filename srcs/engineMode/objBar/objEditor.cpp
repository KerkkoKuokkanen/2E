
#include "objEditor.h"
#include "imgui.h"
#include "transform.h"
#include "customComponent.h"

void ObjectEditor::SecureDeleteButton(SystemObj *obj)
{
	static double holdStartTime = 0.0;
	static bool isHolding = false;
	const double holdDuration = 0.5; // Require 0.5 seconds hold

	// Render button (but don't use it for activation)
	ImGui::Button("Delet Component##Hold", ImVec2(120, 20));

	// Check if button is currently being held down
	if (ImGui::IsItemActive()) 
	{
		if (!isHolding) 
		{
			holdStartTime = ImGui::GetTime(); // Start timer only once
			isHolding = true;
		}

		double elapsed = ImGui::GetTime() - holdStartTime;
		float progress = elapsed / holdDuration;

		if (elapsed >= holdDuration) // Hold long enough
		{
			if (selectWindow == n_ComponentTypes::IMAGE_CLASS)
			{
				t_sysComponent comp = obj->components[compIndex];
				Image *img = (Image*)comp.obj;
				delete (img);
			}
			else
			{
				t_sysComponent comp = obj->components[compIndex];
				CustomComponent *cust = (CustomComponent*)comp.obj;
				delete (cust);
			}
			selectWindow = 0;
			isHolding = false; // Reset state
		}
	}
	else 
	{
		// Reset if released early
		isHolding = false;
	}
}

void ObjectEditor::TransformUpdate(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Transform");
	Transform *trans = (Transform*)obj->GetComponent("transform");
	t_Point pos = trans->GetPosition();
	float w = trans->GetWidth();
	float h = trans->GetHeight();
	float a = trans->GetAngle();

	ImGui::Text("Position:");
	ImGui::InputFloat("X", &pos.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Y", &pos.y, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Size:");
	ImGui::InputFloat("Width", &w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Height", &h, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Rotation:");
	ImGui::InputFloat("Angle", &a, 0.01f, 1.0f, "%.2f");

	trans->Position(pos.x, pos.y);
	trans->Angle(a);
	trans->Width(w);
	trans->Height(h);
}

void ObjectEditor::ComponentSelector(SystemObj *obj)
{
	ImGui::Text("Components:");
	if (ImGui::Button("Add Component"))
		selectWindow = -1;
	for (int i = 0; i < obj->components.size(); i++)
	{
		t_sysComponent comp = obj->components[i];
		std::string str = comp.type + " " + std::to_string(i);
		if (ImGui::Button(str.c_str()))
		{
			selectWindow = comp.classType;
			compIndex = i;
		}
	}
	ImGui::NewLine();
	ImGui::Text("Layer Manager:");
	if (ImGui::Button("Layer Options"))
		selectWindow = -2;
}

void ObjectEditor::LayerManager()
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Layer Manager");
	ImGui::NewLine();
	const char* options[] = { "No Sort", "Y-Sort", "Depth Sort", "Depth & Y-Sort", "Text Layer", "Multi Sprite"};
	static int currentIndex = 0;
	static int layerNumber = 0;

	ImGui::Text("Add New Layer:");
	ImGui::InputInt("Number", &layerNumber, 1, 1);
	int selectedValue = 0;
	if (ImGui::Combo("Type", &currentIndex, options, IM_ARRAYSIZE(options)))
	{
		switch (currentIndex)
		{
			case 0: selectedValue = n_SortTypes::NO_SORT; break;
			case 1: selectedValue = n_SortTypes::Y_SORT; break;
			case 2: selectedValue = n_SortTypes::DEPTH_SORT; break;
			case 3: selectedValue = n_SortTypes::DEPTH_Y_SORT; break;
			case 4: selectedValue = n_SortTypes::TEXT_LAYER; break;
			case 5: selectedValue = n_SortTypes::MULTI_LAYER; break;
			default: selectedValue = -1; break;
		}
	}
	if (ImGui::Button("Add Layer"))
		universalRenderingSystem.AddLayer(layerNumber, currentIndex);

	ImGui::NewLine();
	ImGui::Text("Remove Layer:");
	std::vector<std::tuple<int, int>> data = universalRenderingSystem.GetLayerData();
	static int currentIndex2 = 0;

	std::vector<std::string> items;
	for (const auto& entry : data) {
		items.push_back("Layer: " + std::to_string(std::get<0>(entry)) + 
						"  Type: " + options[std::get<1>(entry)]);
	}

	std::vector<const char*> itemPtrs;
	for (const auto& item : items)
		itemPtrs.push_back(item.c_str());

	if (ImGui::Combo("Layer", &currentIndex2, itemPtrs.data(), itemPtrs.size()))
	{
		int selectedID = std::get<0>(data[currentIndex2]);
		int selectedValue = std::get<1>(data[currentIndex2]);
	}
	bool returnVal = true;
	if (ImGui::Button("Remove"))
		returnVal = universalRenderingSystem.RemoveLayer(std::get<0>(data[currentIndex2]));
	if (returnVal == false)
		printf("Layer contains data or did not exits. Could not remove.\n");
}

void ObjectEditor::LayerSelectionDropDown(SystemObj *obj)
{
	t_sysComponent com = obj->components[compIndex];
	Image *img = (Image*)com.obj;
	std::vector<std::tuple<int, int>> data = universalRenderingSystem.GetLayerData();
	const char* options[] = { "No Sort", "Y-Sort", "Depth Sort", "Depth & Y-Sort", "Text Layer", "Multi Sprite"};
	int currentIndex = img->GetLayer();

	std::vector<std::string> items;
	for (const auto& entry : data) {
		items.push_back("Layer: " + std::to_string(std::get<0>(entry)) + 
						"  Type: " + options[std::get<1>(entry)]);
	}

	std::vector<const char*> itemPtrs;
	for (const auto& item : items) {
		itemPtrs.push_back(item.c_str());
	}

	if (ImGui::Combo("Layer", &currentIndex, itemPtrs.data(), itemPtrs.size()))
	{
		// Extract selected tuple values
		int selectedID = std::get<0>(data[currentIndex]);
		int selectedValue = std::get<1>(data[currentIndex]);
	}
	img->SetLayer(std::get<0>(data[currentIndex]));
}

void ObjectEditor::UpdateImageClass(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Image:");
	t_sysComponent com = obj->components[compIndex];
	Image *img = (Image*)com.obj;

	t_Point pos = img->GetPosition();
	float a = img->GetAngle();
	float w = img->GetWidht();
	float h = img->GetHeight();
	int layer = img->GetLayer();
	t_Box c = img->GetColor();

	SetImageTexture(obj);

	ImGui::Text("Position:");
	ImGui::InputFloat("X", &pos.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Y", &pos.y, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Size:");
	ImGui::InputFloat("Width", &w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Height", &h, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Rotation:");
	ImGui::InputFloat("Angle", &a, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Color:");
	ImGui::InputFloat("Red", &c.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Green", &c.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Blue", &c.w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Alpha", &c.h, 0.01f, 1.0f, "%.2f");

	ImGui::NewLine();
	LayerSelectionDropDown(obj);

	img->SetPosition(pos.x, pos.y);
	img->SetAngle(a);
	img->SetWidth(w);
	img->SetHeight(h);
	img->SetColor(c.x, c.y, c.w, c.h);

	ImGui::NewLine();
	SecureDeleteButton(obj);
}

void ObjectEditor::UpdateSelectedWindow(SystemObj *obj)
{
	switch (selectWindow)
	{
		case -1:
			ComponentAdder(obj);
			break ;
		case -2:
			LayerManager();
			break ;
		case 0:
			ComponentSelector(obj);
			break ;
		case n_ComponentTypes::TRANSFORM_CLASS:
			TransformUpdate(obj);
			break ;
		case n_ComponentTypes::IMAGE_CLASS:
			UpdateImageClass(obj);
			break ;
		case n_ComponentTypes::STRUCTURE_CLASS:
			break ;
		default:
			UpdateCustomComponent(obj);
			break ;
	}
}

void ObjectEditor::NoSelectWindow()
{
	ImGui::Text("Object Not Selected");
	ImGui::End();
}

void ObjectEditor::UpdateSelected(uint64_t key, bool selected, std::string name)
{
	ImGui::Begin("Object Editor");
	if (selected == false)
		return (NoSelectWindow());
	SystemObj *obj = FindSystemObject(key);
	if (obj == NULL)
		return (NoSelectWindow());
	if (lastSelected != obj->GetSystemObjectKey())
	{
		lastSelected = obj->GetSystemObjectKey();
		compIndex = 0;
		selectWindow = 0;
	}
	name += ":";
	ImGui::Text("%s", name.c_str());
	UpdateSelectedWindow(obj);
	ImGui::End();
}
