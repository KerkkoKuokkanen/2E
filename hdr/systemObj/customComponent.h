
#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"

# define DEFAULT_SAVE_SIZE 256

class CustomComponent
{
	private:
		void *initData = NULL;
		size_t initDataSize = 0;
		size_t totalSize = 0;
		bool usingCustomPool = false;
		void InitializeMemory();
		void ReallocateMemory(size_t addSize);
	protected:
		void AddToSaveData(void *addition, size_t addSize);
	public:
		SystemObj *self = NULL;
		virtual ~CustomComponent();
		size_t GetComponentSize() {return (initDataSize);};
		void *CollectSaveData(size_t &size) {
			size = initDataSize;
			return (initData);
		};
		virtual void Init(void *data) {};
		virtual void Update() {};
		virtual void Destroy() {};
};

#endif
