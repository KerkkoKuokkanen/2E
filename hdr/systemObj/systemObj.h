
#ifndef SYSTEM_OBJ_H
# define SYSTEM_OBJ_H

# include <vector>
# include <iostream>

namespace n_ComponentTypes
{
	//Keep the image classes last
	enum {
		CUSTOM_CLASS,
		IMAGE_CLASS,
		STRUCTURE_CLASS
	};
};


typedef struct s_sysComponent
{
	int classType;
	const char *type;
	void *obj;
}				t_sysComponent;

class SystemObj
{
	private:
		unsigned int uniqueSystemObjKey = 0;
		std::vector<t_sysComponent> components = {};
	public:
		SystemObj();
		unsigned int GetSystemObjectKey() {return (uniqueSystemObjKey);};
		void *GetComponent(const char *component);
		std::vector<void*> GetComponents(const char *components);
		void AddComponent(const char *component);
		void UpdateSystemObj();
};

#endif
