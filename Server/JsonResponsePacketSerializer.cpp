#include "JsonResponsePacketSerializer.h"

JsonResponsePacketSerializer* JsonResponsePacketSerializer::m_instance = nullptr;
Lock JsonResponsePacketSerializer::m_lock;

/// <summary>
/// The function getts the only instance of the static
/// class JsonResponsePacketSerializer.
/// </summary>
/// <returns> The instance of the class.</returns>
JsonResponsePacketSerializer* JsonResponsePacketSerializer::getInstance()
{
    return nullptr;	lock_guard<Lock> lockGuard(m_lock);
    if (m_instance == nullptr)
    {
        m_instance = new JsonResponsePacketSerializer();
    }
    return m_instance;
}

/// <summary>
/// The function makes an ErrorResponse variable to a Buffer.
/// </summary>
/// <param name="errResp"> The ErrorResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the ErrorResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errResp)
{
    Buffer* buffer = new Buffer();
    string responseData = echoFormat(getField("message", errResp.message));

    return *makeBuffer(ERROR_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a LoginResponse variable to a Buffer.
/// </summary>
/// <param name="logResp"> The LoginResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the LoginResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const LoginResponse& logResp)
{
    string responseData = echoFormat(getField("status", logResp.status));

    return *makeBuffer(LOGIN_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a SignUpResponse variable to a Buffer.
/// </summary>
/// <param name="signUpResp"> The SignUpResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the SignUpResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const SignUpResponse& signUpResp)
{
    string responseData = echoFormat(getField("status", signUpResp.status));

    return *makeBuffer(SIGN_UP_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a LogOutResponse variable to Buffer.
/// </summary>
/// <param name="logOutResp">The response with the data</param>
/// <returns> a reference to the Buffer- the new Buffer with the data of the LogoutResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logOutResp)
{
    string responseData = echoFormat(getField("status", logOutResp.status));

    return *makeBuffer(LOGOUT_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a LogOutResponse variable to Buffer.
/// </summary>
/// <param name="getRoomsResp">The response with the data</param>
/// <returns>a reference to the Buffer- the new Buffer with the data of the GetRoomsResponse</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResp)
{
    string responseData = getField("status", getRoomsResp.status) + SEPERATOR;
    string roomsData = "";
    int count = 1;

    //Getting the data from the rooms of the GetRoomsResp
    for (auto i = getRoomsResp.rooms.begin(); i != getRoomsResp.rooms.begin(); i++)
    {
        roomsData += getField("room" + to_string(count), getRoomDataString(*i)) + SEPERATOR;
    }
    roomsData.pop_back(); //removing the last SEPERATOR
    responseData += echoFormat(roomsData);
    return *makeBuffer(GET_ROOMS_RESP_CODE, echoFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResp)
{
    string responseData = getPlayersString(getPlayersInRoomResp.players);
    return *makeBuffer(GET_PLAYERS_IN_ROOM_RESP_CODE, responseData);
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResp)
{
    string responseData = echoFormat(getField("status", joinRoomResp.status));
    return *makeBuffer(JOIN_ROOM_RESP_CODE, responseData);
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResp)
{
    string responseData = echoFormat(getField("status", createRoomResp.status));
    return *makeBuffer(CREATE_ROOM_RESP_CODE, responseData);
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& getHighScoreResp)
{
    string responseData = getField("status", getHighScoreResp.status);
    string statisticsData = getStatisticsString(getHighScoreResp.statistics);
    responseData += SEPERATOR + statisticsData;
    return *makeBuffer(GET_HIGH_SCORE_RESP_CODE, echoFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& getPersonStatsResp)
{
    string responseData = getField("status", getPersonStatsResp.status);
    string statisticsData = getStatisticsString(getPersonStatsResp.statistics);
    responseData += SEPERATOR + statisticsData;
    return *makeBuffer(GET_PERS_STATS_RESP_CODE, echoFormat(responseData));
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
    int initLen = paddedNum.length();

    //Padding the number
    for (int i = initLen; i < digits; i++)
    {
        paddedNum = ZERO_CHAR + paddedNum;
    }
    return paddedNum;
}

/// <summary>
/// The function makes a vector of players to a string
/// </summary>
/// <param name="players"> The vector with the data</param>
/// <returns> The string from the vector</returns>
string JsonResponsePacketSerializer::getPlayersString(const vector<string>& players)
{
    string playersStr = "";
    int count = 1;

    //Going over the players's vector
    for (auto i = players.begin(); i != players.end(); i++)
    {
        playersStr += getField("player" + to_string(count), *i) + SEPERATOR;
        count++;
    }
    playersStr.pop_back(); // removing the last SEPERATOR
    return echoFormat(playersStr);
}

/// <summary>
/// The function makes a vector of statistics to a string
/// </summary>
/// <param name="players"> The vector with the data</param>
/// <returns> The string from the vector</returns>
string JsonResponsePacketSerializer::getStatisticsString(const vector<string>& statistics)
{
    string playersStr = "";
    int count = 1;

    //Going over the statistics's vector
    for (auto i = statistics.begin(); i != statistics.end(); i++)
    {
        playersStr += getField("statistic" + to_string(count), *i) + SEPERATOR;
        count++;
    }
    playersStr.pop_back(); // removing the last SEPERATOR
    return echoFormat(playersStr);
}

/// <summary>
/// The function extract the data from a RoomFata struct
/// to a string.
/// </summary>
/// <param name="roomData"> The RoomData with the data</param>
/// <returns> The string with the data of the room.</returns>
string JsonResponsePacketSerializer::getRoomDataString(const RoomData& roomData)
{
    string roomDataStr = getField("id", roomData.id) + SEPERATOR;
    roomDataStr += getField("name", roomData.name) + SEPERATOR;
    roomDataStr += getField("maxPlayers", roomData.maxPlayers) + SEPERATOR;
    roomDataStr += getField("numOfQuestionsInGame", roomData.numOfQuestionsInGame) + SEPERATOR;
    roomDataStr += getField("timePerQuestions", roomData.timePerQuestions) + SEPERATOR;
    roomDataStr += getField("isActive", roomData.isActive) + SEPERATOR;

    return echoFormat(roomDataStr);
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

/// <summary>
/// The function makes a string to be like 
/// the format of json.
/// </summary>
/// <param name="str"> The data to format.</param>
/// <returns> The formated data</returns>
string JsonResponsePacketSerializer::echoFormat(const string str)
{
    return "{" + str + "}";
}

/// <summary>
/// The function makes a buffer from a data
/// and its code.
/// </summary>
/// <param name="code"> The code of the buffer</param>
/// <param name="responseData"> The data to insert to the buffer</param>
/// <returns> The address of the new buffer.</returns>
Buffer* JsonResponsePacketSerializer::makeBuffer(const RESPONSE_CODES code, string responseData)
{
    Buffer* buffer = new Buffer();
    addStringToBuffer(buffer, to_string(code)); //adding the code
    addStringToBuffer(buffer, getPaddedNumber(responseData.length(), SIZE_LENGTH_DATA_FIELD)); //addding the size of the message
    addStringToBuffer(buffer, responseData); //adding the message
    buffer->push_back('\0'); //end of the buffer
    return buffer;
}

