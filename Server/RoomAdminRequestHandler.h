#pragma once

#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

class RequestHandlerFactory;
class RoomMemberRequestHandler;

class RoomAdminRequestHandler : public RoomMemberRequestHandler
{
public:
	//C`tors
	RoomAdminRequestHandler() = default;
	RoomAdminRequestHandler(const Room& m_room, const LoggedUser& m_user, RoomManager& m_roomManager, RequestHandlerFactory* m_handlerFactory);
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo);
	RequestResult& handleRequest(const RequestInfo& requestInfo);

private:
	//Help functions
	RequestResult& closeRoom(const RequestInfo& requestInfo);
	RequestResult& startGame(const RequestInfo& requestInfo);
};

