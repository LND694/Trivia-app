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
    string responseData = echoJsonFormat(getField<string>("message", errResp.message));

    return *makeBuffer(ERROR_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a LoginResponse variable to a Buffer.
/// </summary>
/// <param name="logResp"> The LoginResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the LoginResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const LoginResponse& logResp)
{
    string responseData = echoJsonFormat(getField<unsigned int>("status", to_string(logResp.status)));

    return *makeBuffer(LOGIN_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a SignUpResponse variable to a Buffer.
/// </summary>
/// <param name="signUpResp"> The SignUpResponse with the data.</param>
/// <returns> a reference to Buffer- the Buffer with the data of the SignUpResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const SignUpResponse& signUpResp)
{
    string responseData = echoJsonFormat(getField<unsigned int>("status", to_string(signUpResp.status)));

    return *makeBuffer(SIGN_UP_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a LogOutResponse variable to Buffer.
/// </summary>
/// <param name="logOutResp">The response with the data</param>
/// <returns> a reference to the Buffer- the new Buffer with the data of the LogoutResponse.</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logOutResp)
{
    string responseData = echoJsonFormat(getField<unsigned int>("status", to_string(logOutResp.status)));

    return *makeBuffer(LOGOUT_RESP_CODE, responseData);
}

/// <summary>
/// The function makes a LogOutResponse variable to Buffer.
/// </summary>
/// <param name="getRoomsResp">The response with the data</param>
/// <returns>a reference to the Buffer- the new Buffer with the data of the GetRoomsResponse</returns>
Buffer& JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResp)
{
    string responseData = getField<unsigned int>("status", to_string(getRoomsResp.status)) + SEPERATOR;
    string roomsData = "";

    //Getting the data from the rooms of the GetRoomsResp
    for (auto i = getRoomsResp.rooms.begin(); i != getRoomsResp.rooms.end(); i++)
    {
        roomsData += echoJsonFormat(getRoomDataString(*i)) + SEPERATOR;
    }
    if (!roomsData.empty()) // there is not only the opening char for the vector
    {
        roomsData.pop_back(); // removing the last SEPERATOR
    }
    responseData += getField<vector<RoomData>>("Rooms", echoVectorJsonFormat(roomsData));
    return *makeBuffer(GET_ROOMS_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResp)
{
    string responseData = echoJsonFormat(getField<string>("PlayersInRoom", getVectorString(getPlayersInRoomResp.players)));
    return *makeBuffer(GET_PLAYERS_IN_ROOM_RESP_CODE, responseData);
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResp)
{
    string responseData = echoJsonFormat(getField<unsigned int>("status", to_string(joinRoomResp.status)));
    return *makeBuffer(JOIN_ROOM_RESP_CODE, responseData);
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResp)
{
    string responseData = echoJsonFormat(getField<unsigned int>("status", to_string(createRoomResp.status)));
    return *makeBuffer(CREATE_ROOM_RESP_CODE, responseData);
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& getHighScoreResp)
{
    string responseData = getField<unsigned int>("status", to_string(getHighScoreResp.status));
    string statisticsData = getVectorString(getHighScoreResp.statistics);
    statisticsData = getField<vector<unsigned int>>("statistics", echoVectorJsonFormat(statisticsData));
    responseData += SEPERATOR + statisticsData;
    return *makeBuffer(GET_HIGH_SCORE_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& getPersonStatsResp)
{
    string responseData = getField<unsigned int>("status", to_string(getPersonStatsResp.status));
    string statisticsData = getVectorString(getPersonStatsResp.statistics);
    responseData += SEPERATOR + statisticsData;
    return *makeBuffer(GET_PERS_STATS_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& closeRoomResp)
{
    string responseData = getField<unsigned int>("status", to_string(closeRoomResp.status));
    return *makeBuffer(CLOSE_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& startGameResp)
{
    string responseData = getField<unsigned int>("status", to_string(startGameResp.status));
    return *makeBuffer(START_GAME_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& leaveRoomResp)
{
    string responseData = getField<unsigned int>("status", to_string(leaveRoomResp.status));
    return *makeBuffer(LEAVE_ROOM_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& getRoomStateResp)
{
    string playersData = "[";
    string condition = getRoomStateResp.hasGameBegun ? "true" : "false";
    string responseData = getField<unsigned int>("status", to_string(getRoomStateResp.status));
    responseData += SEPERATOR + getField<bool>("hasGameBegun", condition);

    //Adding the names of the players in the room
    for (const auto& i : getRoomStateResp.players)
    {
        playersData += echoStringJsonFormat(i)+ SEPERATOR;
    }
    if (getRoomStateResp.players.size() > 0)
    {
        playersData.pop_back();
    }
    playersData += ']';
    responseData += SEPERATOR + getField<vector<string>>("players", playersData);
    responseData += SEPERATOR + getField<unsigned int>("questionCount", to_string(getRoomStateResp.questionCount));
    responseData += SEPERATOR + getField<unsigned int>("answerTimeOut", to_string(getRoomStateResp.answerTimeOut));
    return *makeBuffer(GET_ROOM_STATE_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& getGameResResp)
{
    string responseData = getField<unsigned int>("status", to_string(getGameResResp.status)) + SEPERATOR;
    string results = "";

    //Going over the results's vector
    for (auto i = getGameResResp.results.begin(); i != getGameResResp.results.end(); i++)
    {
        results += echoJsonFormat(getPlayerResultsString(*i)) + SEPERATOR;
    }
    if (results.length() > 0)
    {
        results.pop_back();
    }

    responseData += getField<vector<PlayerResults>>("results", echoVectorJsonFormat(results));

    return *makeBuffer(GET_GAME_RES_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& subAnswerResp)
{
    string responseData = getField<unsigned int>("status", to_string(subAnswerResp.status)) + SEPERATOR;
    responseData += getField<unsigned int>("correctAnswerId", to_string(subAnswerResp.correctAnswerId));

    return *makeBuffer(SUBMIT_ANSWER_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& getQuestionResp)
{
    string answersData = "";
    string responseData = getField<unsigned int>("status", to_string(getQuestionResp.status)) + SEPERATOR;
    responseData += getField<string>("question", getQuestionResp.question) + SEPERATOR;

    //Going over the answers
    for (auto i = getQuestionResp.answers.begin(); i != getQuestionResp.answers.end(); i++)
    {
        answersData += getMapField(i->first, i->second) + SEPERATOR;
    }
    if (answersData.length() > 0)
    {
        answersData.pop_back();
    }

    responseData += getField<map<unsigned int, string>>("answers", echoJsonFormat(answersData));
    return *makeBuffer(GET_QUESTION_RESP_CODE, echoJsonFormat(responseData));

}

Buffer& JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& leaveGameResp)
{
    string responseData = getField<unsigned int>("status", to_string(leaveGameResp.status));
    return *makeBuffer(LEAVE_GAME_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const AddQuestionResponse& addQuestResp)
{
    string responseData = getField<unsigned int>("status", to_string(addQuestResp.status));
    return *makeBuffer(ADD_QUESTION_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const GetStateHeadResponse& stateHeadResp)
{
    string responseData = getField<unsigned int>("status", to_string(stateHeadResp.status));
    responseData += SEPERATOR + getField<unsigned int>("amountPlayers", to_string(stateHeadResp.amountPlayers));
    return *makeBuffer(ADD_QUESTION_RESP_CODE, echoJsonFormat(responseData));
}

Buffer& JsonResponsePacketSerializer::serializeResponse(const HeadModeResponse& headModeResp)
{
    string responseData = getField<unsigned int>("status", to_string(headModeResp.status));
    return *makeBuffer(ADD_QUESTION_RESP_CODE, echoJsonFormat(responseData));
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
    int initLen = static_cast<int>(paddedNum.length());

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
string JsonResponsePacketSerializer::getVectorString(const vector<string>& vec)
{
    string vectorStr = "";

    //Going over the players's vector
    for (auto i = vec.begin(); i != vec.end(); i++)
    {
        vectorStr += *i + SEPERATOR;
    }
    vectorStr.pop_back(); // removing the last SEPERATOR
    return vectorStr;
}


/// <summary>
/// The function extract the data from a RoomFata struct
/// to a string.
/// </summary>
/// <param name="roomData"> The RoomData with the data</param>
/// <returns> The string with the data of the room.</returns>
string JsonResponsePacketSerializer::getRoomDataString(const RoomData& roomData)
{
    string roomDataStr = getField<unsigned int>("id", to_string(roomData.id)) + SEPERATOR;
    roomDataStr += getField<string>("name", roomData.name) + SEPERATOR;
    roomDataStr += getField<unsigned int>("maxPlayers", to_string(roomData.maxPlayers)) + SEPERATOR;
    roomDataStr += getField<unsigned int>("numOfQuestionsInGame", to_string(roomData.numOfQuestionsInGame)) + SEPERATOR;
    roomDataStr += getField<unsigned int>("timePerQuestion", to_string(roomData.timePerQuestion)) + SEPERATOR;
    roomDataStr += getField<unsigned int>("isActive", to_string(roomData.isActive));

    return roomDataStr;
}

/// <summary>
/// The function gets from a PlayerResults struct its
/// string which sums up its properties.
/// </summary>
/// <param name="playerResults"> The struct with the data.</param>
/// <returns> The string which sums up the data.</returns>
string JsonResponsePacketSerializer::getPlayerResultsString(const PlayerResults& playerResults)
{
    string playerResultsStr = getField<string>("username", playerResults.username) + SEPERATOR;
    playerResultsStr += getField<unsigned int>("correctAnswerCount", to_string(playerResults.correctAnswerCount)) + SEPERATOR;
    playerResultsStr += getField<unsigned int>("wrongAnswerCount", to_string(playerResults.wrongAnswerCount)) + SEPERATOR;
    playerResultsStr += getField<unsigned int>("averageAnswerTime", to_string(playerResults.averageAnswerTime));

    return playerResultsStr;
}

/// <summary>
/// The function gets the field from
/// a key and a value of a map.
/// </summary>
/// <param name="key"> The key of the map.</param>
/// <param name="value"> The value of the map.</param>
/// <returns> The field as a string.</returns>
string JsonResponsePacketSerializer::getMapField(unsigned int key, string value)
{
    return to_string(key) + ":" + echoStringJsonFormat(value);
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
string JsonResponsePacketSerializer::echoJsonFormat(const string str)
{
    return "{" + str + "}";
}

/// <summary>
/// The function echos a Json String format.
/// </summary>
/// <param name="str"> The string without the format.</param>
/// <returns> The string with the format.</returns>
string JsonResponsePacketSerializer::echoStringJsonFormat(const string str)
{
    return '"' + str + '"';
}

/// <summary>
/// The function echos a Json Vector format.
/// </summary>
/// <param name="str"> The string without the format.</param>
/// <returns> The string with the format.</returns>
string JsonResponsePacketSerializer::echoVectorJsonFormat(const string str)
{
    return "[" + str + "]";
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
    addStringToBuffer(buffer, getPaddedNumber(static_cast<int>(responseData.length()), SIZE_LENGTH_DATA_FIELD)); //addding the size of the message
    addStringToBuffer(buffer, responseData); //adding the message
    buffer->push_back('\0'); //end of the buffer
    return buffer;
}

