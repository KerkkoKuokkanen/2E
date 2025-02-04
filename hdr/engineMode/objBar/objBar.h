
#ifndef OBJ_BAR_H
# define OBJ_BAR_H

# include "customComponent.h"
# include "componentRegistry.h"

class ObjBar : public CustomComponent
{
	private:
		Image *selectImg = NULL;
		void ObjectsSelector(std::unordered_map<uint64_t, SystemObj*> &objs);
		bool save = true;
	public:
		ObjBar();
		~ObjBar() override;
		void Init(void *data, size_t size) override;
		void Update() override;
};

REGISTER_COMPONENT(ObjBar);

#endif
