#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
protected:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory* m_handlerFactory;

	RequestResult& getRoomState(const RequestInfo& requestInfo);
	void createErrorResponse(string msg, RequestResult* reqRes);

public:
	//c`tor
	RoomMemberRequestHandler(const Room m_room, const LoggedUser m_user, RoomManager& m_roomManager, RequestHandlerFactory* m_handlerFactory);
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;

private:
	RequestResult& leaveRoom(const RequestInfo& requestInfo);
};

