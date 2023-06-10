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
unsigned int RoomManager::getRoomState(RoomId id)
{
	return getRoom(id).getRoomData().isActive == false;
}

/// <summary>
/// get a vector of the active rooms's data
/// </summary>
/// <returns> rooms data</returns>
vector<RoomData>& RoomManager::getRooms() const
{
	vector<RoomData>* roomsData = new vector<RoomData>();

	//Going over the rooms
	for (auto i : this->m_rooms)
	{
		if (!i.second.getRoomData().isActive)
		{
			roomsData->push_back(i.second.getRoomData());
		}
	}
	return *roomsData;
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
