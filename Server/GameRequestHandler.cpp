#include "GameRequestHandler.h"

bool GameRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return false;
}

RequestResult& GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    return *reqRes;
}
