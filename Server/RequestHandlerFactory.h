#pragma once

#include "LoginManager.h"

class RequestHandlerFactory
{
public:
	LoginManager& getLoginManager();

private:
	IDatabase* m_dataBase;
	LoginManager m_loginManager;
};

