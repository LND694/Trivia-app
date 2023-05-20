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
    Buffer* data = getDataFromBuffer(buffer);
    std::string dataToParse(data->begin(), data->end());
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
    Buffer* data = getDataFromBuffer(buffer);
    std::string dataToParse(data->begin(), data->end());
    auto js = json::parse(dataToParse); //parse into json object

    req->username = js[NAME_KEY];
    req->password = js[PASSOWRD_KEY];
    req->email = js[EMAIL_KEY];
    req->address = js[ADDRESS_KEY];
    req->phoneNum = js[PHONE_NUM_KEY];
    req->bornDate = js[BORN_DATE_KEY]; //defines stored in global.h

    delete data;
    return *req;
}

GetPlayersInRoomRequest& JsonRequestPacketDeserializer::desrializeGetPlayersRequest(const Buffer& buffer)
{
    GetPlayersInRoomRequest* req = new GetPlayersInRoomRequest();
    Buffer* data = getDataFromBuffer(buffer);
    std::string dataToParse(data->begin(), data->end());
    auto js = json::parse(dataToParse); //parse into json object
    req->roomId = js[ROON_ID];//defines stored in global.h

    delete data;
    return *req;
}

JoinRoomRequest& JsonRequestPacketDeserializer::desrializeJoinRoomRequest(const Buffer& buffer)
{
    JoinRoomRequest* req = new JoinRoomRequest();
    Buffer* data = getDataFromBuffer(buffer);
    std::string dataToParse(data->begin(), data->end());
    auto js = json::parse(dataToParse); //parse into json object
    req->roomId = js[ROON_ID];//defines stored in global.h

    delete data;
    return *req;
}

CreateRoomRequest& JsonRequestPacketDeserializer::desrializeCreateRoomRequest(const Buffer& buffer)
{
    CreateRoomRequest* req = new CreateRoomRequest();
    Buffer* data = getDataFromBuffer(buffer);
    std::string dataToParse(data->begin(), data->end());
    auto js = json::parse(dataToParse); //parse into json object

    req->answerTimeout = js[ANSWER_TIMEOUT];
    req->maxUsers = js[MAX_USERS];
    req->questionCount = js[QUESTION_COUNT];
    req->roomName = js[ROOM_NAME];//defines stored in global.h
    delete data;
    return *req;
}

/// <summary>
/// The function getts the segment of the data 
/// and inserting it into another Buffer.
/// </summary>
/// <param name="buf"> The Buffer of the full request.</param>
/// <returns> a pointer to a Buffer variable- the data only.</returns>
Buffer* JsonRequestPacketDeserializer::getDataFromBuffer(const Buffer& buf)
{
    Buffer* data = new Buffer();
    unsigned char currentChar = 0;
    
    //Going over the buffer, starting from the data segment
    for (int i = SIZE_CODE_FIELD + SIZE_LENGTH_DATA_FIELD + 2; i < buf.size();i++)
    {
        currentChar = buf[i];
        //if the character is not a letter or a scope
        if (currentChar != SPACE && currentChar != NEW_LINE && currentChar != END_STR_SYMBOL)
        {
            data->push_back(currentChar);
        }
    }
    return data;
}
