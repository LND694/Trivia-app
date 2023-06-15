#include "RoomMemberRequestHandler.h"


/// <summary>
/// c`tor for room member handler
/// </summary>
/// <param name="m_room"> the room of the user</param>
/// <param name="m_user"> the user itself</param>
/// <param name="m_roomManager"> room manager</param>
/// <param name="m_handlerFactory"> handler factory </param>
RoomMemberRequestHandler::RoomMemberRequestHandler(const Room m_room, const LoggedUser m_user, RoomManager& m_roomManager, RequestHandlerFactory* m_handlerFactory)
    : m_room(m_room), m_user(m_user), m_roomManager(m_roomManager), m_handlerFactory(m_handlerFactory)
{
}


/// <summary>
/// checks if the request is relevent
/// </summary>
/// <param name="requestInfo"> the request from the client</param>
/// <returns></returns>
bool RoomMemberRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return requestInfo.id == LEAVE_ROOM_REQS_CODE || requestInfo.id == GET_ROOM_STATE_REQS_CODE;
}

/// <summary>
/// handle the request
/// </summary>
/// <param name="requestInfo"> the request of the client</param>
/// <returns> request result</returns>
RequestResult& RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.id == LEAVE_ROOM_REQS_CODE)
    {
        return leaveRoom(requestInfo);
    }
    return getRoomState(requestInfo);
}

/// <summary>
/// disconnect the user from the room
/// </summary>
/// <param name="requestInfo"> the request from the client</param>
/// <returns> request result</returns>
RequestResult& RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo)
{
    LeaveRoomResponse leaveResp = LeaveRoomResponse();
    RequestResult* reqRes = new RequestResult();//the result to return
    if (isRequestRelevent(requestInfo))
    {
        try
        {
            this->m_roomManager.getRoom(this->m_room.getRoomData().id).removeUser(this->m_user);
            this->m_room = this->m_roomManager.getRoom(this->m_room.getRoomData().id);
        }
        catch (...)
        {

        }

        leaveResp.status = OK_STATUS_CODE;
        reqRes->response = JsonResponsePacketSerializer::serializeResponse(leaveResp);
        reqRes->newHandler = this->m_handlerFactory->createMenuRequestHandler(this->m_user);//return the user to the menu
    }
    else 
    {
        createErrorResponse(ERROR_MSG, reqRes);
    }
    return *reqRes;
}



/// <summary>
/// get the user room state
/// </summary>
/// <param name="requestInfo"> the request of the client</param>
/// <returns> request result</returns>
RequestResult& RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    GetRoomStateResponse stateResp = GetRoomStateResponse();
    if (isRequestRelevent(requestInfo))
    {
        try
        {
            //Creating the GetRoomStateResponse
            this->m_room = this->m_roomManager.getRoom(this->m_room.getRoomData().id);
            stateResp.hasGameBegun = this->m_room.getRoomData().isActive;
            stateResp.questionCount = this->m_room.getRoomData().numOfQuestionsInGame;
            stateResp.answerTimeOut = this->m_room.getRoomData().timePerQuestion;
            stateResp.players = this->m_room.getAllUsers();
            stateResp.status = OK_STATUS_CODE;

            reqRes->response = JsonResponsePacketSerializer::serializeResponse(stateResp);

            if (stateResp.hasGameBegun) //the game has already begun
            {
                reqRes->newHandler = this->m_handlerFactory->createGameRequestHandler(this->m_user, this->m_room.getRoomData().id);
            }
            else
            {
                reqRes->newHandler = this;
            }
        }
        catch (const std::exception& excp)
        {
            createErrorResponse("Room not exists", reqRes);
        }
    }
    else
    {
        createErrorResponse(ERROR_MSG, reqRes);
    }
    return *reqRes;
}

/// <summary>
/// creates ERROR result for the client
/// </summary>
/// <param name="errMsg"> the error message to dispaly</param>
/// <param name="reqRes"> the request result to return</param>
void RoomMemberRequestHandler::createErrorResponse(string errMsg, RequestResult* reqRes)
{
    ErrorResopnse errResp;
    errResp.message = errMsg;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);//turn the error message into buffer
    reqRes->newHandler = this;//if there is a error the new handler will be the current
}


