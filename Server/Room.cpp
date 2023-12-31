#include "Room.h"

/// <summary>
/// c`tor for room
/// </summary>
/// <param name="data"> the data for the room</param>
/// <param name="user"> the creator of the room</param>
Room::Room(const RoomData& data, const LoggedUser& user)
{
	this->m_metadata = data;
	this->m_users.push_back(user);
}

/// <summary>
/// add user to the list of users in the room
/// </summary>
/// <param name="user"> the user to add</param>
void Room::addUser(const LoggedUser& user)
{
	//Checking the current amount of the players in the room
	if (this->m_metadata.isActive)
	{
		throw std::exception("This room is not activated");
	}
	if (this->m_users.size() == this->m_metadata.maxPlayers)
	{
		throw std::exception("There is no more place in this room");
	}

	this->m_users.push_back(user);
}


/// <summary>
/// remove a user from the list of users
/// </summary>
/// <param name="user"> the user to remove</param>
void Room::removeUser(LoggedUser& user)
{
	//using lambda function beacuse stl doesnt know how to handle LoggedUser:
	std::erase_if(this->m_users, [&user](LoggedUser& player) {return user.getUsername() == player.getUsername(); });//erase_if is better (c++20 feature)
}

vector<string>& Room::getAllUsers() const
{
	vector<string>* res = new vector<string>();
	for (const auto& i : this->m_users)
	{
		res->push_back(i.getUsername());
	}
	return *res;
}

RoomData& Room::getRoomData() const
{
	RoomData* roomData = new RoomData();
	//Copying the metadata
	roomData->id = this->m_metadata.id;
	roomData->isActive = this->m_metadata.isActive;
	roomData->name = this->m_metadata.name;
	roomData->maxPlayers = this->m_metadata.maxPlayers;
	roomData->numOfQuestionsInGame = this->m_metadata.numOfQuestionsInGame;
	roomData->timePerQuestion = this->m_metadata.timePerQuestion;
	return *roomData;
}

/// <summary>
/// The function activates the room.
/// </summary>
void Room::activeRoom()
{
	this->m_metadata.isActive = 1;
}
