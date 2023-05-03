#include "JsonResponsePacketSerializer.h"
#include <cmath>

using NLOHMANN_JSON_NAMESPACE::json;

/// <summary>
/// The function makes an ErrorResponse variable to a Buffer.
/// </summary>
/// <param name="errResp"> The ErrorResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the ErrorResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errResp)
{
    Buffer* buffer = new Buffer();
    string responseData = "{\"message\": \"" + errResp.message + "\"}";
    json msgJson = responseData;

    //Making the Buffer
    addNumToBuffer(buffer, ERROR_RESP_CODE);
    addNumToBuffer(buffer, responseData.length());
    buffer->push_back(msgJson);

    return *buffer;
    
}

/// <summary>
/// The function makes a LoginResponse variable to a Buffer.
/// </summary>
/// <param name="errResp"> The LoginResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the LoginResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const LoginResponse& logResp)
{
    Buffer* buffer = new Buffer();
    string responseData = "{\"status\": \"" + to_string(logResp.status) + "\"}";
    json msgJson = responseData;

    //Making the Buffer
    addNumToBuffer(buffer, LOGIN_RESP_CODE);
    addNumToBuffer(buffer, responseData.length());
    buffer->push_back(msgJson);

    return *buffer;
}

/// <summary>
/// The function makes a SignUpResponse variable to a Buffer.
/// </summary>
/// <param name="errResp"> The SignUpResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the SignUpResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const SignUpResponse& signResp)
{
    Buffer* buffer = new Buffer();
    string responseData = "{\"status\": \"" + to_string(signResp.status) + "\"}";
    json msgJson = responseData;

    //Making the Buffer
    addNumToBuffer(buffer, SIGN_UP_RESP_CODE);
    addNumToBuffer(buffer, responseData.length());
    buffer->push_back(msgJson);

    return *buffer;
}

/// <summary>
/// Th function inserts a number to the Buffer.
/// </summary>
/// <param name="buf">The buf to insert to</param>
/// <param name="num"> The num to add</param>
void JsonResponsePacketSerializer::addNumToBuffer(Buffer* buf, const int num)
{
    int digit = 0;

    //Going over the digits of the code
    for (int i = 0; i < getAmountDigits(num); i++)
    {
        buf->push_back(to_string(num)[i]);
    }
}

/// <summary>
/// The function gets the amount of the 
/// digits of a number.
/// </summary>
/// <param name="num"> The number</param>
/// <returns>The amount of its digits.</returns>
int JsonResponsePacketSerializer::getAmountDigits(const int num)
{
    int theNum = num, count = 0;

    //Going over the digits in the num
    while (theNum != 0)
    {
        count++;
        theNum /= 10;
    }
    return count;
}

