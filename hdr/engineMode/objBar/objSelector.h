
#ifndef OBJECT_SELECTOR_H
# define OBJECT_SELECTOR_H

# include "systemObj.h"

struct NodeData {
	char name[48];
	bool is_folder;
	int parent_id;
	uint64_t objKey;
};

class ObjectSelector
{
	public:
		std::tuple<uint64_t, bool, std::string> UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self);
		std::vector<NodeData> CollectObjSelectorData();
		void InitializeObjectSelector(std::vector<NodeData> &data);
};

#endif
