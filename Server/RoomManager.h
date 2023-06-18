#pragma once
#include "Room.h"
#include <map>
using std::map;
typedef unsigned int RoomId;

class RoomManager
{
public:
	//Functions
	void createRoom(const LoggedUser& user, const RoomData& data);
	void deleteRoom(const RoomId id);
	unsigned int getRoomState(const RoomId id);
	vector<RoomData>& getRooms() const;
	Room& getRoom(const RoomId id);
private:
	//Field
	map<RoomId, Room> m_rooms;
};

