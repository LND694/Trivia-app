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
    addStringToBuffer(buffer, to_string(ERROR_RESP_CODE)); //adding the code
    addStringToBuffer(buffer, getPaddedNumber(responseData.length(), SIZE_LENGTH_DATA_FIELD)); //addding the size of the message
    addStringToBuffer(buffer, to_string(msgJson));
    buffer->push_back('\0');

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

    //Making the buffer
    addStringToBuffer(buffer, to_string(LOGIN_RESP_CODE)); //adding the code
    addStringToBuffer(buffer, getPaddedNumber(responseData.length(), SIZE_LENGTH_DATA_FIELD)); //addding the size of the message
    buffer->push_back(msgJson); //adding the msg
    buffer->push_back('\0');

    return *buffer;
}

/// <summary>
/// The function makes a SignUpResponse variable to a Buffer.
/// </summary>
/// <param name="errResp"> The SignUpResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the SignUpResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const SignUpResponse& signUpResp)
{
    Buffer* buffer = new Buffer();
    string responseData = "{\"status\": " + to_string(signUpResp.status) + "}";
    json msgJson = responseData;

    //Making the buffer
    addStringToBuffer(buffer, to_string(SIGN_UP_RESP_CODE)); //adding the code
    addStringToBuffer(buffer, getPaddedNumber(responseData.length(), SIZE_LENGTH_DATA_FIELD)); //addding the size of the message
    addStringToBuffer(buffer, responseData);
    buffer->push_back('\0');

    return *buffer;
}

/// <summary>
/// The function add to the number '0' to
/// tits left side to fill it to a 
/// specific amount of digits.
/// </summary>
/// <param name="num">The num to pad.</param>
/// <param name="digits">The final amount of digits.</param>
/// <returns>a string value- the padded number as string.</returns>
string JsonResponsePacketSerializer::getPaddedNumber(const int num, const int digits)
{
    string paddedNum = to_string(num);

    //Padding the number
    while (paddedNum.length() < digits)
    {
        paddedNum = '0' + paddedNum;
    }
    return paddedNum;
}

/// <summary>
/// The function adds the string to the buffer.
/// </summary>
/// <param name="buf">The Buffer to insert to.</param>
/// <param name="str"> The string to insert to the Buffer.</param>
void JsonResponsePacketSerializer::addStringToBuffer(Buffer* buf, string str)
{
    //Going over the string
    for (int i = 0; i < str.length(); i++)
    {
        buf->push_back(str[i]);
    }
}

