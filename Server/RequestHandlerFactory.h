#pragma once

#include "LoginManager.h"
#include "LoginRequestHandler.h"

class LoginRequestHandler;

class RequestHandlerFactory
{
public:
	//C'tor
	RequestHandlerFactory(IDatabase* db, LoginManager logMan);

	//Functions
	LoginRequestHandler* createLoginRequestHandler() const;
	LoginManager& getLoginManager();

private:
	//Fields
	IDatabase* m_dataBase;
	LoginManager m_loginManager;
};

