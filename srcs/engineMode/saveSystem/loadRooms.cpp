
#include <iostream>
#include <filesystem>
#include <string>
#include "saveInterface.h"
#include "commonTools.h"
#include "envHandler.h"
#include <thread>
#include <mutex>

namespace fs = std::filesystem;

std::mutex roomMutex;

std::unordered_map<uint16_t, std::string> roomsWithKeys;
std::unordered_map<std::string, uint16_t> keysWithRooms;

std::string GetRoomWithKey(uint16_t key)
{
	std::lock_guard<std::mutex> guard(roomMutex);
	return (roomsWithKeys[key]);
}

std::string GetRoomWithName(std::string name)
{
	std::lock_guard<std::mutex> guard(roomMutex);
	uint16_t key = keysWithRooms[name];
	return (roomsWithKeys[key]);
}

bool LoadRoomObjects(uint16_t room)
{
	uint32_t key = SetAskedData(GetRoomWithKey(room) + "/data");
	void *data = NULL;
	for (int i = 0; i < 60; i++)
	{
		bool checker = CollectAskedState(key, &data);
		if (checker)
			break ;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (data == NULL)
		return (false);
	SnapShot snap = MakeIntoSnapshot(data);
	LoadObjectsToEnvironment(snap, room);
	free(data);
	free(snap.data);
	return (true);
}

void LoadRooms()
{
	std::lock_guard<std::mutex> guard(roomMutex);
	uint16_t roomKey = 1;
	for (const auto& entry : fs::recursive_directory_iterator("saves/rooms"))
	{
		if (fs::is_directory(entry.path()))
		{
			std::string filepath = entry.path().string();
			std::string dirName = entry.path().stem().string(); 
			if (entry.path().extension() != ".rf")
			{
				if (dirName == "engineRoom")
				{
					roomsWithKeys[0] = filepath;
					keysWithRooms[dirName] = 0;
				}
				else
				{
					roomsWithKeys[roomKey] = filepath;
					keysWithRooms[dirName] = roomKey;
					roomKey += 1;
				}
			}
		}
	}
}
