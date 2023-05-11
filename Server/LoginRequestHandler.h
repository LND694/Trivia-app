#pragma once
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"


//class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	//C'tor
	LoginRequestHandler();

	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo);
	RequestResult& handleRequest(const RequestInfo& requestInfo);
	RequestResult& login(const RequestInfo& requestInfo);
	RequestResult& signUp(const RequestInfo& requestInfo);
private:
	IDatabase* dataBase;
	RequestHandlerFactory* m_handlerFactory;//reference must be intilized but there is no c`tor fo handlerFactory
};

