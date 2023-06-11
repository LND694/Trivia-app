#include "JsonRequestPacketDeserializer.h"

JsonRequestPacketDeserializer* JsonRequestPacketDeserializer::m_instance = nullptr;
Lock JsonRequestPacketDeserializer::m_lock;

/// <summary>
/// The function gett the only instance of the class JsonRequestPacketDeserializer.
/// </summary>
/// <returns>The only instance of the class JsonRequestPacketDeserializer.</returns>
JsonRequestPacketDeserializer* JsonRequestPacketDeserializer::getInstance()
{
    lock_guard<Lock> lockGuard(m_lock);
    if (m_instance == nullptr)
    {
        m_instance = new JsonRequestPacketDeserializer();
    }
    return m_instance;
}

/// <summary>
/// deserializer for buffer into login request
/// </summary>
/// <param name="buffer"> the buffer the client sent</param>
/// <returns>new created login request</returns>
LoginRequest& JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
    LoginRequest* req = new LoginRequest();
    std::string dataToParse(buffer.begin(), buffer.end());
    dataToParse.erase(0, SIZE_LENGTH_DATA_FIELD + SIZE_CODE_FIELD);
    auto js = json::parse(dataToParse);//parse into json object


    req->username = js[NAME_KEY];
    req->password = js[PASSOWRD_KEY];//defines stored in global.h
    return *req;
}


/// <summary>
/// deserializer forr the buffer into signup request
/// </summary>
/// <param name="buffer"> the buffer the client sent</param>
/// <returns>new created signup request</returns>
SignupRequest& JsonRequestPacketDeserializer::desrializeSignupRequest(const Buffer& buffer)
{
    SignupRequest* req = new SignupRequest();
    std::string dataToParse(buffer.begin(), buffer.end());
    dataToParse.erase(0, SIZE_LENGTH_DATA_FIELD+SIZE_CODE_FIELD);
    auto js = json::parse(dataToParse); //parse into json object

    req->username = js[NAME_KEY];
    req->password = js[PASSOWRD_KEY];
    req->email = js[EMAIL_KEY];
    req->address = js[ADDRESS_KEY];
    req->phoneNum = js[PHONE_NUM_KEY];
    req->bornDate = js[BORN_DATE_KEY]; //defines stored in global.h

    return *req;
}



GetPlayersInRoomRequest& JsonRequestPacketDeserializer::desrializeGetPlayersRequest(const Buffer& buffer)
{
    GetPlayersInRoomRequest* req = new GetPlayersInRoomRequest();
    std::string dataToParse(buffer.begin(), buffer.end());
    auto js = json::parse(dataToParse); //parse into json object
    req->roomId = js[ROOM_ID];//defines stored in global.h

    return *req;
}

JoinRoomRequest& JsonRequestPacketDeserializer::desrializeJoinRoomRequest(const Buffer& buffer)
{
    JoinRoomRequest* req = new JoinRoomRequest();
    std::string dataToParse(buffer.begin(), buffer.end());
    dataToParse.erase(0, SIZE_LENGTH_DATA_FIELD + SIZE_CODE_FIELD);
    auto js = json::parse(dataToParse); //parse into json object
    req->roomId = js[ROOM_ID];//defines stored in global.h

    return *req;
}

CreateRoomRequest& JsonRequestPacketDeserializer::desrializeCreateRoomRequest(const Buffer& buffer)
{
    CreateRoomRequest* req = new CreateRoomRequest();
    std::string dataToParse(buffer.begin(), buffer.end());
    dataToParse.erase(0, SIZE_LENGTH_DATA_FIELD + SIZE_CODE_FIELD);
    auto js = json::parse(dataToParse); //parse into json object

    req->answerTimeout = js[ANSWER_TIMEOUT];
    req->maxUsers = js[MAX_USERS];
    req->questionCount = js[QUESTION_COUNT];
    req->roomName = js[ROOM_NAME];//defines stored in global.h

    return *req;
}

SubmitAnswerRequest& JsonRequestPacketDeserializer::derializeSubmitAnswerRequest(const Buffer& buffer)
{
    SubmitAnswerRequest* req = new SubmitAnswerRequest();
    string dataToParse(buffer.begin(), buffer.end());
    dataToParse.erase(0, SIZE_LENGTH_DATA_FIELD + SIZE_CODE_FIELD);
    auto js = json::parse(dataToParse); //parse into json object
    req->answerId = js[ANSWER_ID];
    return *req;
}


