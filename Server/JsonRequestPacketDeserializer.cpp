
#include "JsonRequestPacketDeserializer.h"


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
    auto js = json::parse(dataToParse);
    req->username = js[NAME_KEY];//parse into json object
    req->password = js[PASSOWRD_KEY];
    req->email = js[EMAIL_KEY];//defines stored in global.h

    delete data;
    return *req;
}

Buffer* JsonRequestPacketDeserializer::getDataFromBuffer(const Buffer& buf)
{
    Buffer* data = new Buffer();
    unsigned char currentChar = 0;
    for (int i = 15; i < buf.size();i++)
    {
        currentChar = buf[i];
        if (currentChar != ' ' && currentChar != '\n' && currentChar != '\0')
        {
            data->push_back(currentChar);
        }
    }
    return data;
}
