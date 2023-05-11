#pragma once
#include "IRequestHandler.h"


class MenuRequestHandler : public IRequestHandler
{
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;
};

