#include "RoomAdminRequestHandler.h"

bool RoomAdminRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return LEAVE_ROOM_REQS_CODE == requestInfo.id || GET_ROOM_STATE_REQS_CODE == requestInfo.id;
}

RequestResult& RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult* req = new RequestResult();

    if (!isRequestRelevent(requestInfo))
    {
        createErrorResponse(ERROR_MSG, req);
        return *req;
    }

    if (CLOSE_ROOM_REQS_CODE == requestInfo.id)
    {
        *req = closeRoom(requestInfo);
    }
    else if (GET_ROOM_STATE_REQS_CODE == requestInfo.id)
    {
        *req = getRoomState(requestInfo);
    }
    else if(START_GAME_REQS_CODE == requestInfo.id)
    {
        *req = startGame(requestInfo);
    }
    return *req;
}

RequestResult& RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    CloseRoomResponse closeResp = CloseRoomResponse();
    RequestResult* reqRes = new RequestResult();//the result to return
    unsigned int idRoom = this->m_room.getRoomData().id;

    //Closing the room
    this->m_roomManager.deleteRoom(idRoom);
    closeResp.status = OK_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(closeResp);
    reqRes->newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);//return the user to the menu

    return *reqRes;
}

RequestResult& RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    StartGameResponse startGameResp = StartGameResponse();
    RequestResult* reqRes = new RequestResult();//the result to return

    //"Starting" the game
    startGameResp.status = OK_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(startGameResp);
    reqRes->newHandler = new GameRequestHandler();

    return *reqRes;
}
