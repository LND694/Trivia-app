#pragma once

#include "IRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(const LoggedUser& loggedUser, RoomManager& roomManager,
		StatisticsManager& statisticsManager, RequestHandlerFactory* handlerFactory);
	~MenuRequestHandler();
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;
private:
	//Help functions
	void createErrorResponse(const string errMsg, RequestResult* reqRes);

	RequestResult& signout(const RequestInfo& requestInfo);
	RequestResult& getRooms(const RequestInfo& requestInfo);
	RequestResult& getPlayersInRoom(const RequestInfo& requestInfo);
	RequestResult& getPersonalStats(const RequestInfo& requestInfo);
	RequestResult& getHighScore(const RequestInfo& requestInfo);
	RequestResult& joinRoom(const RequestInfo& requestInfo);
	RequestResult& createRoom(const RequestInfo& requestInfo);

	//Fields
	LoggedUser m_user;
	RoomManager& m_roomManager;
	StatisticsManager& m_statisticsManager;
	RequestHandlerFactory* m_handlerFactory;
};

