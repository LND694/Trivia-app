#include "RequestHandlerFactory.h"





RequestHandlerFactory::RequestHandlerFactory(IDatabase* db, LoginManager logMan) :
    m_dataBase(db), m_loginManager(logMan)
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() const
{
    return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
