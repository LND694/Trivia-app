#pragma once

#include "LoginManager.h"
#include "LoginRequestHandler.h"

class LoginRequestHandler;

class RequestHandlerFactory
{
public:
	//C'tor
	RequestHandlerFactory(IDatabase* db, LoginManager logMan);

	LoginRequestHandler* createLoginRequestHandler() const;
	LoginManager& getLoginManager();

private:
	IDatabase* m_dataBase;
	LoginManager m_loginManager;
};

