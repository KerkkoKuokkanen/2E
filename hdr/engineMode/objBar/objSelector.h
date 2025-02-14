
#ifndef OBJECT_SELECTOR_H
# define OBJECT_SELECTOR_H

# include "systemObj.h"

class ObjectSelector
{
	public:
		void UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self);
};

#endif
