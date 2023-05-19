#include "Room.h"

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
	this->m_users.push_back(user);
}


/// <summary>
/// remove a user from the list of users
/// </summary>
/// <param name="user"> the user to remove</param>
void Room::removeUser(const LoggedUser& user)
{
	std::remove(this->m_users.begin(), this->m_users.end(), user);
}


/// <summary>
/// get a vector of the names of the users in the room
/// </summary>
/// <returns> the names of the users in the room</returns>
vector<string> Room::getAllUsers() const
{
	vector<string> res = vector<string>();
	for (auto i : this->m_users)
	{
		res.push_back(i.getUsername());
	}
	return res;
}

RoomData& Room::getRoomData() const
{
	return this->m_metadata;
}
