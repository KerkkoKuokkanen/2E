
#ifndef ROOM_LOADING_H
# define ROOM_LOADING_H

void LoadRooms();
std::string GetRoomWithKey(uint16_t key);
std::string GetRoomWithName(std::string name);
bool LoadRoomObjects(uint16_t room);

#endif
