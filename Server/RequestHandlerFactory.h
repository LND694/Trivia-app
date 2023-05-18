#pragma once

#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "global.h"
#include "Singleton.h"

class LoginRequestHandler;

class RequestHandlerFactory : Singleton
{
public:
	static RequestHandlerFactory* getInstance(IDatabase* db, LoginManager* logMan);
	RequestHandlerFactory(RequestHandlerFactory& other) = delete;
	void operator=(const RequestHandlerFactory& other) = delete;
	//Functions
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager* getLoginManager();

protected:
	//C'tor
	RequestHandlerFactory(IDatabase* db, LoginManager* logMan);

	//Singleton fields
	static RequestHandlerFactory* m_instance;
	static Lock m_lock;
private:
	//Fields
	IDatabase* m_dataBase;
	LoginManager* m_loginManager;
};

