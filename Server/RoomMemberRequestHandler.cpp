#include "RoomMemberRequestHandler.h"

bool RoomMemberRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    return requestInfo.id == LEAVE_ROOM_REQS_CODE ||requestInfo.id == GET_ROOMS_REQS_CODE;
}

RequestResult& RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.id == LEAVE_ROOM_REQS_CODE)
    {
        return leaveRoom(requestInfo);
    }
}

RequestResult& RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo)
{
    LeaveRoomResponse leaveResp = LeaveRoomResponse();
    RequestResult* reqRes = new RequestResult();
    this->m_room.removeUser(this->m_user);
    leaveResp.status = OK_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(leaveResp);
    reqRes->newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
    return *reqRes;
}

RequestResult& RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();
    GetRoomStateResponse stateResp = GetRoomStateResponse();
    stateResp.hasGameBegun = this->m_roomManager.getRoomState(this->m_room.getRoomData().id);
    stateResp.AnswerCount = this->m_room.getRoomData().numOfQuestionsInGame;
    stateResp.answerTimeOut = this->m_room.getRoomData().timePerQuestion;
    stateResp.players = this->m_room.getAllUsers();
    stateResp.status = OK_CODE;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(stateResp);
    reqRes->newHandler = this;
    return *reqRes;
}
