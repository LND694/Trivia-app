#pragma once
#include "LoggedUser.h"
#include <vector>
using std::vector;

typedef struct RoomData
{
	unsigned int id{};
	string name{};
	unsigned int maxPlayers{};
	unsigned int numOfQuestionsInGame{};
	unsigned int timePerQuestion{};
	unsigned int isActive{};
}RoomData;



class Room
{
public:
	//C'tor
	Room(const RoomData& data, const LoggedUser& user);

	//Functions
	void addUser(const LoggedUser& user);
	void removeUser(LoggedUser& user);
	vector<string>& getAllUsers() const;
	RoomData& getRoomData() const;
	void activeRoom();

private:
	//Fields
	RoomData m_metadata;
	vector<LoggedUser> m_users;
};


