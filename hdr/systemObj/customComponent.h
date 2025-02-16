
#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"

# define DEFAULT_SAVE_SIZE 256

typedef std::tuple<void*, size_t> tracking;

//Custom component is the class that most scripts should inherit from
//SystemObj automatically updates and takes care of CustomComponents memory
//The class offers all the basic functions you need and the possibility for saving the component

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
		//Will be called after each component of SystemObj *self has performed the Init function
		virtual void Start() {};

		//Update: self explainatory
		virtual void Update() {};

		//Update function that is called after updates and rendering
		virtual void LastUpdate() {};

		//This function is called right before the object gets deleted
		virtual void Destroy() {};
};

#endif
