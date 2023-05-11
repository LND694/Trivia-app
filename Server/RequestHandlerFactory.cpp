#include "RequestHandlerFactory.h"





LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
