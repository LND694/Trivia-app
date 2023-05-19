#pragma once
#include "Room.h"
#include <map>
using std::map;
typedef unsigned int RoomId;

class RoomManager
{
public:
	void createRoom(const LoggedUser& user,RoomData& data);
	void deleteRoom(const RoomId id);
	unsigned int getRoomState(const RoomId id);
	vector<RoomData>& getRooms() const;
	Room& getRoom(const RoomId id);
private:
	map<RoomId, Room> m_rooms;
};

