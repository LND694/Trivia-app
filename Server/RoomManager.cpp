#include "RoomManager.h"



/// <summary>
/// insert a new Room to the rooms map
/// </summary>
/// <param name="user"> the creator of the room</param>
/// <param name="data"> the room data</param>
void RoomManager::createRoom(const LoggedUser& user, RoomData& data)
{
	this->m_rooms.insert({ data.id, Room(data, user)});
}


/// <summary>
/// delete the room with the id
/// </summary>
/// <param name="id"></param>
void RoomManager::deleteRoom(const RoomId id)
{
	this->m_rooms.erase(id);
}

/// <summary>
/// get room state
/// </summary>
/// <param name="id"> the id of the room</param>
/// <returns> the room state</returns>
RoomId RoomManager::getRoomState(RoomId id)
{
	return getRoom(id).getRoomData().isActive;
}

/// <summary>
/// get a vector of the rooms data
/// </summary>
/// <returns> rooms data</returns>
vector<RoomData>& RoomManager::getRooms() const
{
	vector<RoomData> roomsData = vector<RoomData>();
	for (auto i : this->m_rooms)
	{
		roomsData.push_back(i.second.getRoomData());
	}
	return roomsData;
}

/// <summary>
/// get a specific room
/// </summary>
/// <param name="id"> the id of the room to get</param>
/// <returns> the room with that id</returns>
Room& RoomManager::getRoom(const RoomId id)
{
	return this->m_rooms.at(id);
}