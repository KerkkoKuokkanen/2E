
#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"

# define DEFAULT_SAVE_SIZE 256

typedef std::tuple<void*, size_t> tracking;

class CustomComponent
{
	private:
		void *initData = NULL;
		size_t initDataSize = 0;
		std::vector<tracking> saveTracking = {};
	protected:
		void AddToSave(void *addition, size_t addSize);
	public:
		uint32_t ownId = 0;
		SystemObj *self = NULL;
		virtual ~CustomComponent();
		size_t GetComponentSize() {return (initDataSize);};
		void *CollectSaveData(size_t &size);

		//Init is called right after the component is created
		//Should not rely on any other components or try to get components
		virtual void Init(void *data, size_t size) {};

		//Start is called once and guanranteed to happen before the first update
		//Use this for what the init can not do like getting other components from self
		virtual void Start() {};

		//Update: self explainatory
		virtual void Update() {};

		//Virtual destroy if you want to use it instead of destructor
		virtual void Destroy() {};
};

#endif
