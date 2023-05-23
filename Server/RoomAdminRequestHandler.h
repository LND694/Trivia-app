#pragma once
#include "IRequestHandler.h"

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;
};

