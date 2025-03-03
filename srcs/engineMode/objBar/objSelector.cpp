
#include "objSelector.h"
#include "imgui.h"
#include <unordered_map>
#include <optional>
#include <cstring>
#include <set>
#include "envHandler.h"
#include <math.h>

// Unique ID generator for nodes
static int next_id = 1;
unsigned int objCounter = 0;

// Structure to represent folders and game objects
struct Node {
	int id;
	std::string name;
	bool is_folder;
	std::optional<int> parent_id;
	bool renaming = false;
	char rename_buffer[128] = "";
	uint64_t objKey;

	Node(std::string n, bool folder, std::optional<int> parent = std::nullopt)
		: id(next_id++), name(std::move(n)), is_folder(folder), parent_id(parent) {
		std::strncpy(rename_buffer, name.c_str(), sizeof(rename_buffer));
	}
};

// Global list of objects and folders
std::vector<Node> nodes;
std::optional<int> selected_node_id = std::nullopt;
std::set<int> nodes_to_delete; // Set of nodes to delete after loop

// Find a node by ID
Node* FindNodeById(int id) {
	for (auto& node : nodes) {
		if (node.id == id)
			return &node;
	}
	return nullptr;
}

Node* FindNodeByObjKey(uint64_t key) {
	for (auto& node : nodes) {
		if (node.objKey == key)
			return &node;
	}
	return nullptr;
}

// Delete nodes after loop to prevent crashes
void ProcessDeletions() {
	if (!nodes_to_delete.empty()) {
		for (int id : nodes_to_delete) {
			Node* node = FindNodeById(id);
			if (node) {
				if (node->is_folder == false)
				{
					SystemObj *obj = FindSystemObject(node->objKey);
					if (obj != NULL)
						obj->Destroy();
				}
			}
		}
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
			[](const Node& node) {
				return nodes_to_delete.count(node.id) > 0;
			}),
			nodes.end());
		nodes_to_delete.clear();
	}
}

// Handles renaming logic
void RenderRenameInput(Node& node) {
	ImGui::SetKeyboardFocusHere();
	if (ImGui::InputText("##rename", node.rename_buffer, sizeof(node.rename_buffer),
						ImGuiInputTextFlags_EnterReturnsTrue)) {
		node.name = node.rename_buffer;
		node.renaming = false;
	}
	if (!ImGui::IsItemActive() && ImGui::IsMouseClicked(0)) {
		node.renaming = false;
	}
}

// Render the object hierarchy recursively
void ShowHierarchy(std::optional<int> parent_id = std::nullopt) {
	for (auto& node : nodes) {
		if (node.parent_id != parent_id)
			continue;

		bool selected = (selected_node_id && *selected_node_id == node.id);

		if (node.is_folder) {
			bool open = ImGui::TreeNodeEx(node.name.c_str(), selected ? ImGuiTreeNodeFlags_Selected : 0);

			if (node.renaming) {
				RenderRenameInput(node);
			}

			// Right-click menu
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Rename")) {
					node.renaming = true;
				}
				if (ImGui::MenuItem("Delete")) {
					nodes_to_delete.insert(node.id); // Mark for deletion
				}
				if (ImGui::MenuItem("Create Folder")) {
					std::string used = "New Folder" + std::to_string(objCounter);
					nodes.emplace_back(used.c_str(), true, node.id);
					objCounter += 1;
				}
				if (ImGui::MenuItem("Create Object")) {
					SystemObj *added = new SystemObj();
					std::string used = "New Object" + std::to_string(objCounter);
					nodes.emplace_back(used.c_str(), false, node.id);
					nodes[nodes.size() - 1].objKey = added->GetSystemObjectKey();
					objCounter += 1;
				}
				ImGui::EndPopup();
			}

			if (ImGui::IsItemClicked()) selected_node_id = node.id;

			// Drag & Drop source
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload("NODE_PAYLOAD", &node.id, sizeof(int));
				ImGui::Text("Moving: %s", node.name.c_str());
				ImGui::EndDragDropSource();
			}

			// Drag & Drop target
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD")) {
					int dragged_id = *(int*)payload->Data;
					if (dragged_id != node.id) {
						if (Node* dragged_node = FindNodeById(dragged_id)) {
							dragged_node->parent_id = node.id;
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (open) {
				ShowHierarchy(node.id);
				ImGui::TreePop();
			}
		} else {
			// Render Game Objects
			if (node.renaming) {
				RenderRenameInput(node);
			} else {
				if (ImGui::Selectable(node.name.c_str(), selected))
					selected_node_id = node.id;
			}

			// Right-click menu
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Rename")) node.renaming = true;
				if (ImGui::MenuItem("Delete")) {
					nodes_to_delete.insert(node.id); // Mark for deletion
				}
				ImGui::EndPopup();
			}

			// Drag & Drop source
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload("NODE_PAYLOAD", &node.id, sizeof(int));
				ImGui::Text("Moving: %s", node.name.c_str());
				ImGui::EndDragDropSource();
			}

			// Drag & Drop target
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD")) {
					int dragged_id = *(int*)payload->Data;
					if (dragged_id != node.id) {
						if (Node* dragged_node = FindNodeById(dragged_id)) {
							dragged_node->parent_id = node.parent_id;
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}

// UI Function to show the hierarchy window
void ShowHierarchyWindow() {
	ImGui::Begin("Game Hierarchy");

	// Root-level actions
	if (ImGui::Button("Create Folder")) {
		std::string used = "New Folder" + std::to_string(objCounter);
		nodes.emplace_back(used.c_str(), true);
		objCounter += 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Create Object")) {
		SystemObj *added = new SystemObj();
		std::string used = "New Object" + std::to_string(objCounter);
		nodes.emplace_back(used.c_str(), false);
		nodes[nodes.size() - 1].objKey = added->GetSystemObjectKey();
		objCounter += 1;
	}

	// Dragging items back to root
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD")) {
			int dragged_id = *(int*)payload->Data;
			if (Node* dragged_node = FindNodeById(dragged_id)) {
				dragged_node->parent_id = std::nullopt; // Move back to root
			}
		}
		ImGui::EndDragDropTarget();
	}

	ShowHierarchy();

	ProcessDeletions(); // Delete marked nodes **AFTER** rendering

	ImGui::End();
}

void PutObjsInNodes(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self)
{
	for (auto obj : objs)
	{
		if (obj.first == self)
			continue ;
		if (FindNodeByObjKey(obj.second->GetSystemObjectKey()) == nullptr)
		{
			std::string used = "New Object" + std::to_string(objCounter);
			nodes.emplace_back(used.c_str(), false);
			objCounter += 1;
			nodes[nodes.size() - 1].objKey = obj.second->GetSystemObjectKey();
		}
	}
}

std::vector<NodeData> ObjectSelector::CollectObjSelectorData()
{
	std::vector<NodeData> ret = {};

	for (size_t i = 0; i < nodes.size(); i++)
	{
		NodeData add = {};
		Node &node = nodes[i];
		add.is_folder = node.is_folder;
		add.name = node.name;
		if (!add.is_folder)
			add.objKey = node.objKey;
		else
			add.objKey = 0;
		if (node.parent_id.has_value())
			add.parent_id = *node.parent_id;
		else
			add.parent_id = -1;
		ret.push_back(add);
	}

	return ret;
}

void ObjectSelector::InitializeObjectSelector(std::vector<NodeData> &data)
{
	for (int i = 0; i < data.size(); i++)
	{
		NodeData &node = data[i];
		if (node.parent_id == -1)
			nodes.emplace_back(node.name, node.is_folder);
		else
			nodes.emplace_back(node.name, node.is_folder, node.parent_id);
		Node &nod = nodes[nodes.size() - 1];
		nod.is_folder = node.is_folder;
		nod.objKey = node.objKey;
	}
}

void ObjectSelector::UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self)
{
	PutObjsInNodes(objs, self);
	ShowHierarchyWindow();
}
