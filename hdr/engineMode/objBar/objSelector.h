
#ifndef OBJECT_SELECTOR_H
# define OBJECT_SELECTOR_H

# include "systemObj.h"

struct NodeData {
	std::string name;
	bool is_folder;
	int parent_id;
	uint64_t objKey;
};

class ObjectSelector
{
	public:
		void UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self);
		std::vector<NodeData> CollectObjSelectorData();
		void InitializeObjectSelector(std::vector<NodeData> &data);
};

#endif
