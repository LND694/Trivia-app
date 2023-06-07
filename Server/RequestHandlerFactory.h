#pragma once

#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "global.h"
#include "Singleton.h"

class LoginRequestHandler;
class MenuRequestHandler;


class RequestHandlerFactory : Singleton
{
public:
	static RequestHandlerFactory* getInstance(IDatabase* db, LoginManager* logMan, 
		RoomManager* roomMan, StatisticsManager* statisticsMan);
	RequestHandlerFactory(RequestHandlerFactory& other) = delete;
	void operator=(const RequestHandlerFactory& other) = delete;
	//Functions
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser user);
	LoginManager* getLoginManager();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();

protected:
	//C'tor
	RequestHandlerFactory(IDatabase* db, LoginManager* logMan,
		RoomManager* roomMan, StatisticsManager* statisticsMan);

	//Singleton fields
	static RequestHandlerFactory* m_instance;
	static Lock m_lock;
private:
	//Fields
	IDatabase* m_dataBase;
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
};

