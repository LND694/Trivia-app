#include "RequestHandlerFactory.h"

/// <summary>
/// C'tor of class RequestHandlerFactory
/// </summary>
/// <param name="db">The database</param>
/// <param name="logMan"> The LoginManager of the factory</param>
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db, LoginManager logMan) :
    m_dataBase(db), m_loginManager(logMan)
{
}

/// <summary>
/// The function creates a handler to login requests.
/// </summary>
/// <returns>a LoginRequestHandler value- the handler of the login requests.</returns>
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() const
{
    return new LoginRequestHandler(*this);
}

/// <summary>
/// The function getts the login manager of the factory.
/// </summary>
/// <returns>a reference to LoginManager variable- the login manager.</returns>
LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
