#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
protected:
	//Fields
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory* m_handlerFactory;

	//Help functions
	RequestResult& getRoomState(const RequestInfo& requestInfo);
	void createErrorResponse(string msg, RequestResult* reqRes);

public:
	//c`tor
	RoomMemberRequestHandler(const Room m_room, const LoggedUser m_user, RoomManager& m_roomManager, RequestHandlerFactory* m_handlerFactory);
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;

private:
	//Help function
	RequestResult& leaveRoom(const RequestInfo& requestInfo);
};

