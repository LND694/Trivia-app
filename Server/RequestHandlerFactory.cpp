#include "RequestHandlerFactory.h"

RequestHandlerFactory* RequestHandlerFactory::m_instance = nullptr;
Lock RequestHandlerFactory::m_lock;

/// <summary>
/// C'tor of class RequestHandlerFactory
/// </summary>
/// <param name="db">The database</param>
/// <param name="logMan"> The LoginManager of the factory</param>
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db, LoginManager* logMan) :
    m_dataBase(db), m_loginManager(logMan)
{
}

/// <summary>
/// The function getts the instance of the object RequestFactoryHandler.
/// </summary>
/// <param name="db"> The database for the RequestHandlerFactory.</param>
/// <param name="logMan"> The login manager for the RequestHandlerFactory.</param>
/// <returns> THe instance of the RequestHandlerFactory class.</returns>
RequestHandlerFactory* RequestHandlerFactory::getInstance(IDatabase* db, LoginManager* logMan)
{
	lock_guard<Lock> lockGuard(m_lock);
	if (m_instance == nullptr)
	{
		m_instance = new RequestHandlerFactory(db, logMan);
	}
	return m_instance;
}

/// <summary>
/// The function creates a handler to login requests.
/// </summary>
/// <returns>a LoginRequestHandler value- the handler of the login requests.</returns>
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

/// <summary>
/// The function getts the login manager of the factory.
/// </summary>
/// <returns>a reference to LoginManager variable- the login manager.</returns>
LoginManager* RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
