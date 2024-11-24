
#ifndef SYS_SAVER_H
# define SYS_SAVER_H

# include "systemObj.h"
# include "unordered_map"

# define FETCH_SIZE 1024 * 10
# define SNAPSHOT_AMOUNT 100

struct SaveObjData
{
	uint32_t hash;
	uint32_t componentKey;
	uint32_t componentType;
	size_t compSize;
	void *data;
};

struct SaveObj
{
	uint32_t objHash;
	std::vector<SaveObjData> components;
};

struct SnapObject
{
	size_t objSize;
	uint32_t objHash;
	uint32_t objKey;
	std::vector<SaveObjData*> snapObj;
};

struct SnapShot
{
	uint64_t state;
	uint32_t size;
	void *data;
};

// The Data in the snapShots is formatted like this:
// The data* only holds SystemObjects
// Each system object is formatted like this:
// [uint32_t  ], [uint32_t], [uint32_t    ]
// [unique_key], [hash    ], [size_of_data]
// Each systemObjects data is then formatted like this:
// [uint32_t      ], [uint32_t         ], [size]
// [component_type], [size_of_component], [data]

class SystemSaver
{
	private:
		void *dataFetcher = NULL;
		std::string saveFile;
		std::unordered_map<uint32_t, SaveObj> objectSaves;
		std::vector<SnapShot> snapShots = {};
		void AddNewObject(SystemObj *add);
		int FindFromVector(std::vector<SaveObjData> &components, uint32_t componentKey);
		void CheckExistingObject(SystemObj *check);
		bool HandleExistingObject(SaveObjData &existin, SystemObj *check, SaveObj &current);
		void AddNewComponentToObject(SystemObj *add, SaveObj &newAddition);
		bool CompareToLastSnapShot(void *snap, uint64_t hash);
		void SetSnapObjects(std::vector<SnapObject> &setted, SaveObj &current, size_t &totalSize, uint32_t key);
		void SetToSnapData(uint8_t *snap, std::vector<SnapObject> &saveObjs);
	public:
		SystemSaver();
		~SystemSaver();
		void SetSaveFile(const std::string file);
		void SaveSystemObj(SystemObj *save);
		void TakeSnapShot();
		SystemObj *LoadSystemObj(uint32_t searchKey);
		void RemoveObjectFromSaver(SystemObj *obj);
};

#endif
