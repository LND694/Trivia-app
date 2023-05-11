#include "RequestHandlerFactory.h"



//LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() const
//{
//    return new LoginRequestHandler();
//}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
