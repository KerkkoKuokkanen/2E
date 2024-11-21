
#ifndef SYSTEM_OBJ_H
# define SYSTEM_OBJ_H

# include <vector>
# include <iostream>

# define IMAGE_COMPONENT "image"
# define STRUCTURE_COMPONENT "structure"

namespace n_ComponentTypes
{
	//Keep the image classes first
	enum {
		NO_CLASS,
		STRUCTURE_CLASS,
		IMAGE_CLASS,
		CUSTOM_CLASS
	};
};

typedef struct s_sysComponent
{
	uint32_t uniqueKey;
	uint32_t classType;
	std::string type;
	void *obj;
}				t_sysComponent;

class SystemObj
{
	private:
		int componentSaveFetchIndex = 0;
		uint32_t uniqueSystemObjKey = 0;
		std::vector<t_sysComponent> components = {};
		void ResetComponentSaveFetching() {componentSaveFetchIndex = 0;};
		void IncrimentComponentFetching() {componentSaveFetchIndex += 1;};
		void *FetchComponentSaveData(void *buffer, size_t buffSize, size_t &compSize);
		size_t FetchComponentDataSize();
		uint32_t FetchComponentClassType();
		uint32_t FetchComponentUniqueKey();
		bool ComponentFetchingAtEnd();
		friend class SystemSaver;
	public:
		bool saveable = false;
		void *controller = NULL;

		//Constructor:
		//give the controller as a parameter for automatic handling
		//you can specify the sysEnv or NULL to handle the object manually.
		//saveable only works for when a sysEnv is given.
		SystemObj(void *sysEnv = NULL);
		~SystemObj();
		uint32_t GetSystemObjectKey() {return (uniqueSystemObjKey);};
		void *GetComponent(const std::string &component);
		std::vector<void*> GetComponents(const std::string &components);
		void AddComponentCustom(const std::string component, void *initData);
		void AddComponentStruct(void *component, uint32_t classType, const std::string name);
		void UpdateSystemObj();
};

#endif
