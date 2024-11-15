
#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"

class CustomComponent
{
	private:
		void *initData = NULL;
		size_t initDataSize = 0;
	public:
		SystemObj *self = NULL;
		virtual ~CustomComponent();
		virtual void Init(void *data) {};
		virtual void Update() {};
		virtual void Destroy() {};
};

#endif
