#pragma once
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public RoomMemberRequestHandler
{
public:
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo);
	RequestResult& handleRequest(const RequestInfo& requestInfo);

private:
	RequestResult& closeRoom(const RequestInfo& requestInfo);
	RequestResult& startGame(const RequestInfo& requestInfo);
};

