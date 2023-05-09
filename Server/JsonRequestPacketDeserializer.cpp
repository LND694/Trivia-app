
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
    Buffer* data = getDataFromBuffer(buffer);
    std::string dataToParse(data->begin(), data->end());
    std::cout << "{" << dataToParse << "}" << std::endl;
    auto js = json::parse(dataToParse);
    req->username = js[NAME_KEY];//parse into json object
    req->password = js[PASSOWRD_KEY];
    req->email = js[EMAIL_KEY];//defines stored in global.h

    delete data;
    return *req;
}

Buffer* JsonRequestPacketDeserializer::getDataFromBuffer(const Buffer& buf)
{
    std::vector<unsigned char> jsonData = { 123, 34, 117, 115, 101, 114, 110, 97, 109, 101, 34, 58, 34, 76, 78, 68, 34, 44, 34, 112, 97, 115, 115, 119, 111, 114, 100, 34, 58, 34, 49, 50, 51, 34, 125 };
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
