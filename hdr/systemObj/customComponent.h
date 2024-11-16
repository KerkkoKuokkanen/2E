
#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"

class CustomComponent
{
	private:
		void *initData = NULL;
		size_t initDataSize = 0;
	protected:
		void SetSaveData(void *data, size_t size) {
			initData = data;
			initDataSize = size;
		};
	public:
		SystemObj *self = NULL;
		virtual ~CustomComponent();
		void *CollectSaveData(size_t &size) {
			size = initDataSize;
			return (initData);
		};
		virtual void Init(void *data) {};
		virtual void Update() {};
		virtual void Destroy() {};
};

#endif
