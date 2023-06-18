#pragma once
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "UserInputChecker.h"


class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	//C'tor
	LoginRequestHandler(RequestHandlerFactory* reqHandFac);

	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;
	RequestResult& login(const RequestInfo& requestInfo);
	RequestResult& signUp(const RequestInfo& requestInfo);
private:
	//Field
	RequestHandlerFactory* m_handlerFactory;

	//Help function
	void createErrorResponse(const string errMsg, RequestResult* reqRes);
	static string checkIfInputValid(const SignupRequest& signupReq);
};

