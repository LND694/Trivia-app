#pragma once
//#include "LoginRequestHandler.h"//prevent include probelm
#include "LoginManager.h"



class RequestHandlerFactory
{
public:
	//LoginRequestHandler* createLoginRequestHandler() const; //stop the include problem and dont see a usafe
	LoginManager& getLoginManager();

private:
	IDatabase* m_dataBase;
	LoginManager m_loginManager;
};

