
#ifndef SYS_SAVER_H
# define SYS_SAVER_H

# include "systemObj.h"
# include "unordered_map"

# define FETCH_SIZE 1024 * 10

struct SaveObjData
{
	uint16_t hash;
	uint32_t componentKey;
	size_t compSize;
	void *data;
};

struct SaveObj
{
	uint16_t objHash;
	std::vector<SaveObjData> components;
};

class SystemSaver
{
	private:
		void *dataFetcher = NULL;
		std::unordered_map<uint32_t, SaveObj> objectSaves;
		void AddNewObject(SystemObj *add);
	public:
		SystemSaver();
		~SystemSaver();
		void SaveSystemObj(SystemObj *save);
		SystemObj *LoadSystemObj(uint32_t searchKey);
		void RemoveObjectFromSaver(SystemObj *obj);
};

#endif
