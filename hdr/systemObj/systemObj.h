
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
		STRUCTURE_CLASS,
		IMAGE_CLASS,
		CUSTOM_CLASS
	};
};

typedef struct s_sysComponent
{
	unsigned int classType;
	std::string type;
	void *obj;
}				t_sysComponent;

class SystemObj
{
	private:
		int componentSaveFetchIndex = 0;
		unsigned int uniqueSystemObjKey = 0;
		std::vector<t_sysComponent> components = {};
	public:
		SystemObj();
		~SystemObj();
		unsigned int GetSystemObjectKey() {return (uniqueSystemObjKey);};
		void *GetComponent(const std::string component);
		std::vector<void*> GetComponents(const std::string components);
		void ResetComponentSaveFetching() {componentSaveFetchIndex = 0;};
		void *FetchComponentSaveData(void *buffer, size_t buffSize, size_t &compSize);
		bool ComponentFetchingAtEnd() {
			if (componentSaveFetchIndex >= components.size())
				return (true);
			return (false);
		};
		void AddComponentCustom(const std::string component, void *initData);
		void AddComponentStruct(void *component, unsigned int classType, std::string name);
		void UpdateSystemObj();
};

#endif
