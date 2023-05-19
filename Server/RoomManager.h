#pragma once
#include "Room.h"
#include <map>
using std::map;

class RoomManager
{
public:
	void createRoom(const LoggedUser& user,RoomData& data);
	void deleteRoom(const unsigned int id);
	unsigned int getRoomState(const unsigned int id);
	vector<RoomData> getRooms() const;
	Room& getRoom(const unsigned int id);
private:
	map<unsigned int, Room> m_rooms;
};

