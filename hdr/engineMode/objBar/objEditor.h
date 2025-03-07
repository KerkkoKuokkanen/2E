
#ifndef OBJECT_EDITOR_H
# define OBJECT_EDITOR_H

# include "envHandler.h"

class ObjectEditor
{
	private:
		uint64_t lastSelected = 0;
		int selectWindow = 0;
		int compIndex = 0;
		void TransformUpdate(SystemObj *obj);
		void UpdateSelectedWindow(SystemObj *obj);
		void UpdateCustomComponent(SystemObj *obj);
		void ComponentSelector(SystemObj *obj);
		void ComponentAdder(SystemObj *obj);
		void SecureDeleteButton(SystemObj *obj);
		void UpdateImageClass(SystemObj *obj);
		void SetImageTexture(SystemObj *obj);
		void LayerSelectionDropDown(SystemObj *obj);
		void LayerManager();
		uint64_t GetTextureHash();
		int LayerDropDown();
	public:
		void UpdateSelected(uint64_t key, bool selected, std::string name);
};

#endif
