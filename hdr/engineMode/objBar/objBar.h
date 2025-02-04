
#ifndef OBJ_BAR_H
# define OBJ_BAR_H

# include "customComponent.h"
# include "componentRegistry.h"

class ObjBar : public CustomComponent
{
	private:
		SystemObj *lastSelected = NULL;
		Image *selectImg = NULL;
		std::tuple<SystemObj*, SystemObj*> ObjectsSelector(std::unordered_map<uint64_t, SystemObj*> &objs);
		void ManageSelected();
		void SelectedWindow();
		bool save = true;
	public:
		ObjBar();
		~ObjBar() override;
		void Init(void *data, size_t size) override;
		void Update() override;
};

REGISTER_COMPONENT(ObjBar);

#endif
