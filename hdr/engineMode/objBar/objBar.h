
#ifndef OBJ_BAR_H
# define OBJ_BAR_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "objSelector.h"

class ObjBar : public CustomComponent
{
	private:
		ObjectSelector *objSelect = NULL;
	public:
		ObjBar();
		~ObjBar() override;
		void LastUpdate() override;
};

REGISTER_COMPONENT(ObjBar);

#endif
