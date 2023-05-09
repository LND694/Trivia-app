
#include "JsonRequestPacketDeserializer.h"


/// <summary>
/// deserializer for buffer into login request
/// </summary>
/// <param name="buffer"> the buffer the client sent</param>
/// <returns>new created login request</returns>
LoginRequest& JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
    LoginRequest* req = new LoginRequest();
    auto js = json::parse(buffer);//parse into json object
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
    auto js = json::parse(buffer);
    req->username = js[NAME_KEY];//parse into json object
    req->password = js[PASSOWRD_KEY];
    req->username = js[NAME_KEY];
    req->email = js[EMAIL_KEY];//defines stored in global.h
    return *req;
}
