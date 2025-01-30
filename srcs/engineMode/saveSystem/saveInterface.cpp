
#include "saveInterface.h"
#include "saveSystem.h"
#include <thread>
#include <mutex>

std::mutex functionMutex;
std::vector<std::tuple<SnapShot, std::string>> saveables;
std::string askedFile = "";
void *askedState = NULL;

void *CollectAskedState()
{
	std::lock_guard<std::mutex> guard(functionMutex);
	void *ret = askedState;
	askedState = NULL;
	return (ret);
}

void SetAskedState(void *data)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	askedState = data;
}

void SetAskedData(std::string file)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	askedFile = file;
}

std::string CollectLastFile()
{
	std::lock_guard<std::mutex> guard(functionMutex);
	std::string ret = askedFile;
	askedFile = "";
	return ret;
}

void SaveSnapShot(SnapShot snapShot, std::string file)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	void *data = malloc(snapShot.size);
	memcpy(data, snapShot.data, snapShot.size);
	SnapShot use = {snapShot.hash, snapShot.size, data};
	saveables.push_back({use, file});
}

std::tuple<SnapShot, std::string> CollectFirstFromSnaps()
{
	std::lock_guard<std::mutex> guard(functionMutex);
	if (saveables.size() == 0)
		return ((std::tuple<SnapShot, std::string>){{0, 0, NULL}, ""});
	std::tuple<SnapShot, std::string> ret = saveables[0];
	saveables.erase(saveables.begin() + 0);
	return (ret);
}

void SaveThread()
{
	std::tuple<SnapShot, std::string> ret = CollectFirstFromSnaps();
	SnapShot save = std::get<0>(ret);
	std::string file = std::get<1>(ret);
	if (save.data != NULL)
	{
		void *send = DataPrepping(save.data, save.size, save.hash);
		size_t offset = sizeof(uint64_t);
		uint8_t *castData = (uint8_t*)send;
		uint32_t size = *(uint32_t*)(castData + offset);
		SaveStateToFile(file.c_str(), send, (size_t)size + sizeof(uint32_t) + sizeof(uint64_t));
		free(send);
	}
	std::string loadFile = CollectLastFile();
	if (loadFile != "")
	{
		void *date = LoadStateFromFile(loadFile.c_str());
		if (CorruptionCheck(date) == true)
			SetAskedState(date);
		else
			SetAskedState(NULL);
	}
}
