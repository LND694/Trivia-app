#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
class RoomMemberRequestHandler : public IRequestHandler
{
protected:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;
	RequestResult& getRoomState(const RequestInfo& requestInfo);
public:
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;
private:
	RequestResult& leaveRoom(const RequestInfo& requestInfo);
};

