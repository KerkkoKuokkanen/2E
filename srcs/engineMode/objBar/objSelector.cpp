
#include "objSelector.h"
#include "imgui.h"
#include <optional>
#include <set>
#include "envHandler.h"
#include <math.h>
#include "objectName.h"

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

void MarkChildrenForDeletion(int parent_id) {
	for (auto& node : nodes) {
		if (node.parent_id.has_value() && *node.parent_id == parent_id) {
			nodes_to_delete.insert(node.id); // Mark child for deletion
			MarkChildrenForDeletion(node.id); // Recursively delete children
		}
	}
}

void ProcessDeletions() {
	if (!nodes_to_delete.empty()) {
		// Step 1: Recursively mark children of deleted folders
		std::set<int> nodes_to_process = nodes_to_delete;
		for (int id : nodes_to_process) {
			MarkChildrenForDeletion(id); // Find and mark children
		}

		// Step 2: Destroy all SystemObj instances before deletion
		for (int id : nodes_to_delete) {
			Node* node = FindNodeById(id);
			if (node) {
				SystemObj* obj = FindSystemObject(node->objKey);
				if (obj != nullptr) {
					obj->Destroy(); // ✅ Ensure destruction of game objects
				}
			}
		}

		// Step 3: Actually remove the nodes from the list
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
			[](const Node& node) {
				return nodes_to_delete.count(node.id) > 0;
			}),
			nodes.end());

		// Step 4: Clear the deletion tracking set
		nodes_to_delete.clear();
	}
}


// Handles renaming logic
void RenderRenameInput(Node& node) {
	ImGui::SetKeyboardFocusHere();
	if (ImGui::InputText("##rename", node.rename_buffer, sizeof(node.rename_buffer),
						ImGuiInputTextFlags_EnterReturnsTrue)) {
		node.name = node.rename_buffer;
		uint64_t key = node.objKey;
		SystemObj *obj = FindSystemObject(key);
		ObjectName *name = (ObjectName*)obj->GetComponent("ObjectName");
		if (name == NULL)
		{
			name = new ObjectName();
			obj->AddComponent(name, "ObjectName");
		}
		name->SetName(node.name);
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
					std::string used = "New Folder" + std::to_string(next_id);
					nodes.emplace_back(used.c_str(), true, node.id);
				}
				if (ImGui::MenuItem("Create Object")) {
					SystemObj *added = new SystemObj();
					std::string used = "New Object" + std::to_string(next_id);
					nodes.emplace_back(used.c_str(), false, node.id);
					nodes[nodes.size() - 1].objKey = added->GetSystemObjectKey();
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
				if (!node.is_folder) { // Only for actual objects, not folders
					if (ImGui::MenuItem("Copy Key")) {
						char key_str[32];  // Buffer for the key
						snprintf(key_str, sizeof(key_str), "%llu", node.objKey); // Convert uint64_t to string
						ImGui::SetClipboardText(key_str); // ✅ Copy to clipboard
					}
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

static bool hovered = false;

bool GetObjSelectorHover()
{
	return (hovered);
}

// UI Function to show the hierarchy window
void ShowHierarchyWindow() {
	ImGui::Begin("Object Hierarchy");
	hovered = ImGui::IsWindowHovered();

	// Root-level actions
	if (ImGui::Button("Create Folder")) {
		std::string used = "New Folder" + std::to_string(next_id);
		nodes.emplace_back(used.c_str(), true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Create Object")) {
		SystemObj *added = new SystemObj();
		std::string used = "New Object" + std::to_string(next_id);
		nodes.emplace_back(used.c_str(), false);
		nodes[nodes.size() - 1].objKey = added->GetSystemObjectKey();
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
			ObjectName *name = (ObjectName*)obj.second->GetComponent("ObjectName");
			std::string used = "New Object" + std::to_string(next_id);
			if (name != NULL)
				used = name->GetName() + std::to_string(next_id);
			nodes.emplace_back(used.c_str(), false);
			nodes[nodes.size() - 1].objKey = obj.second->GetSystemObjectKey();
		}
	}
}

std::tuple<uint64_t, bool, std::string> ObjectSelector::UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self)
{
	PutObjsInNodes(objs, self);
	ShowHierarchyWindow();
	if (!selected_node_id.has_value())
		return (std::tuple<uint64_t, bool, std::string>{0, false, ""});
	int id = *selected_node_id;
	Node *node = FindNodeById(id);
	if (node == nullptr)
		return (std::tuple<uint64_t, bool, std::string>{0, false, ""});
	return (std::tuple<uint64_t, bool, std::string>{node->objKey, true, node->name});
}
