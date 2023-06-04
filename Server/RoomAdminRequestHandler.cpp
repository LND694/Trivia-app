#include "RoomAdminRequestHandler.h"

bool RoomAdminRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return false;
}

RequestResult& RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult* req = new RequestResult();
    return *req;
}
