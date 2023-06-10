#include "RoomAdminRequestHandler.h"

/// <summary>
/// C'tor of class RoomAdminRequestHandler
/// </summary>
/// <param name="m_room"> The room which the admin created</param>
/// <param name="m_user"> The admin of the room</param>
/// <param name="m_roomManager">The rooms's manager</param>
/// <param name="m_handlerFactory"> The factory for new reqeust handlers.</param>
RoomAdminRequestHandler::RoomAdminRequestHandler(const Room m_room, const LoggedUser m_user, RoomManager& m_roomManager, RequestHandlerFactory* m_handlerFactory):
    RoomMemberRequestHandler(m_room, m_user, m_roomManager, m_handlerFactory)
{
}

/// <summary>
/// The function checks if the request is
/// even relevent according to its id.
/// </summary>
/// <param name="requestInfo"> The info about the request.</param>
/// <returns>a bool value- if it is relevent or not.</returns>
bool RoomAdminRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return CLOSE_ROOM_REQS_CODE == requestInfo.id || GET_ROOM_STATE_REQS_CODE == requestInfo.id || START_GAME_REQS_CODE == requestInfo.id;
}

/// <summary>
/// The function handles the request.
/// </summary>
/// <param name="requestInfo"> The info about the request.</param>
/// <returns> a RequestResult reference- the result of the request.</returns>
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

/// <summary>
/// The function closes the room which the user is in.
/// </summary>
/// <param name="requestInfo"> The information about the request.</param>
/// <returns> The result of the request.</returns>
RequestResult& RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    CloseRoomResponse closeResp = CloseRoomResponse();
    RequestResult* reqRes = new RequestResult();//the result to return
    unsigned int idRoom = this->m_room.getRoomData().id;

    //Closing the room
    this->m_roomManager.deleteRoom(idRoom);

    closeResp.status = OK_STATUS_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(closeResp);
    reqRes->newHandler = this->m_handlerFactory->createMenuRequestHandler(this->m_user);//return the user to the menu

    return *reqRes;
}

/// <summary>
/// The function starts the game in the room.
/// </summary>
/// <param name="requestInfo"> THe information about the request.</param>
/// <returns> The result from the request.</returns>
RequestResult& RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    StartGameResponse startGameResp = StartGameResponse();
    RequestResult* reqRes = new RequestResult();//the result to return

    //Starting the game
    this->m_roomManager.getRoom(this->m_room.getRoomData().id).getRoomData().isActive = true;
    startGameResp.status = OK_STATUS_CODE;

    reqRes->response = JsonResponsePacketSerializer::serializeResponse(startGameResp);
    reqRes->newHandler = new GameRequestHandler();

    return *reqRes;
}
