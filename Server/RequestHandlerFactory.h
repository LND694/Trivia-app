#pragma once

#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "global.h"
#include "Singleton.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomMemberRequestHandler;
class RoomAdminRequestHandler;


class RequestHandlerFactory : Singleton
{
public:
	static RequestHandlerFactory* getInstance(IDatabase* db, LoginManager* logMan, 
		RoomManager* roomMan, StatisticsManager* statisticsMan, GameManager* gameMan);
	RequestHandlerFactory(RequestHandlerFactory& other) = delete;
	void operator=(const RequestHandlerFactory& other) = delete;
	//Functions to create handlers
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser user);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser user, const Room room);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser user, const Room room);
	GameRequestHandler* createGameRequestHandler(const LoggedUser user, const RoomId roomId);

	//Functions to get managers
	LoginManager* getLoginManager();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	GameManager& getGameManager();

protected:
	//C'tor
	RequestHandlerFactory(IDatabase* db, LoginManager* logMan,
		RoomManager* roomMan, StatisticsManager* statisticsMan, GameManager* gameMan);

	//Singleton fields
	static RequestHandlerFactory* m_instance;
	static Lock m_lock;
private:
	//Fields
	IDatabase* m_dataBase;
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	GameManager* m_gameManager;
};

