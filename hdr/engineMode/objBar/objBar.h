
#ifndef OBJ_BAR_H
# define OBJ_BAR_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "objSelector.h"
# include "objEditor.h"

class ObjBar : public CustomComponent
{
	private:
		ObjectSelector *objSelect = NULL;
		ObjectEditor *objEditor = NULL;
		void CollectObjectSelector();
		void InitObjSelector();
	public:
		ObjBar();
		~ObjBar() override;
		void Init(void *data, size_t size) override;
		void LastUpdate() override;
};

REGISTER_COMPONENT(ObjBar);

#endif
