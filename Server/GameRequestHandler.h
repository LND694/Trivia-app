#pragma once
#include "IRequestHandler.h"

class GameRequestHandler : public IRequestHandler
{
public:
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;
};