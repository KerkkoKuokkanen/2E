
#ifndef OBJ_BAR_H
# define OBJ_BAR_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "objSelector.h"
# include "objEditor.h"
# include "image.h"
# include "mainBar.h"

class ObjBar : public CustomComponent
{
	private:
		ObjectSelector *objSelect = NULL;
		ObjectEditor *objEditor = NULL;
		MainBar *mainBar = NULL;
		Image *img = NULL;
		float angle = 0.0f;

		void CollectObjectSelector();
		void InitObjSelector();
		void ImgUpdate();
	public:
		ObjBar();
		~ObjBar() override;
		bool HoveredOverWindow();
		void Init(void *data, size_t size) override;
		void EngineUpdate() override;
};

REGISTER_COMPONENT(ObjBar);

#endif
