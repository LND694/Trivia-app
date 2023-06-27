#pragma once

#include "Room.h"
#include "global.h"
#include "Response.h"
#include "Singleton.h"

#define ZERO_CHAR '0'
#define SEPERATOR ","

class JsonResponsePacketSerializer : public Singleton
{
public:
	JsonResponsePacketSerializer(JsonResponsePacketSerializer& other) = delete;
	void operator=(const JsonResponsePacketSerializer& other) = delete;
	static JsonResponsePacketSerializer* getInstance();

	//Functions
	static Buffer& serializeResponse(const ErrorResponse& errResp);
	static Buffer& serializeResponse(const LoginResponse& logResp);
	static Buffer& serializeResponse(const SignUpResponse& signUpResp);
	static Buffer& serializeResponse(const LogoutResponse& logOutResp);
	static Buffer& serializeResponse(const GetRoomsResponse& getRoomsResp);
	static Buffer& serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResp);
	static Buffer& serializeResponse(const JoinRoomResponse& joinRoomResp);
	static Buffer& serializeResponse(const CreateRoomResponse& createRoomResp);
	static Buffer& serializeResponse(const GetHighScoreResponse& getHighScoreResp);
	static Buffer& serializeResponse(const GetPersonalStatsResponse& getPersonStatsResp);
	static Buffer& serializeResponse(const CloseRoomResponse& closeRoomResp);
	static Buffer& serializeResponse(const StartGameResponse& startGameResp);
	static Buffer& serializeResponse(const LeaveRoomResponse& leaveRoomResp);
	static Buffer& serializeResponse(const GetRoomStateResponse& getRoomStateResp);
	static Buffer& serializeResponse(const GetGameResultsResponse& getGameResResp);
	static Buffer& serializeResponse(const SubmitAnswerResponse& subAnswerResp);
	static Buffer& serializeResponse(const GetQuestionResponse& getQuestionResp);
	static Buffer& serializeResponse(const LeaveGameResponse& leaveGameResp);
	static Buffer& serializeResponse(const AddQuestionResponse& addQuestResp);
	static Buffer& serializeResponse(const GetStateHeadResponse& stateHeadResp);
	static Buffer& serializeResponse(const HeadModeResponse& headModeResp);

protected:
	JsonResponsePacketSerializer() = default;
	//Singleton fields
	static JsonResponsePacketSerializer* m_instance;
	static Lock m_lock;
private:
	//Help functions
	static string getPaddedNumber(const int num, const int digits);
	static string echoJsonFormat(const string str);
	static string echoStringJsonFormat(const string str);
	static string echoVectorJsonFormat(const string str);
	static Buffer* makeBuffer(const RESPONSE_CODES code, string responseData);
	static void addStringToBuffer(Buffer* buf, string str);

	static string getVectorString(const vector<string>& vec);
	static string getRoomDataString(const RoomData& roomData);
	static string getPlayerResultsString(const PlayerResults& playerResults);
	static string getMapField(unsigned int key, string value);


	template <class T>
	static string getField(const string nameField, const string value);
};

/// <summary>
/// The function combines the key and
/// the value to a field format.
/// </summary>
/// <typeparam name="T"> The real type of the value(what it has)</typeparam>
/// <param name="nameField"> The name of the new field</param>
/// <param name="value"> The value of the key.</param>
/// <returns> The name and the value as a Field format</returns>
template <class T>
inline string JsonResponsePacketSerializer::getField(const string nameField, const string value)
{ 
	string str =  echoStringJsonFormat(nameField) + ":"; //the string needs "'"


	if (typeid(T).name() == typeid(string).name()) // the value is a string
	{
		str += echoStringJsonFormat(value); //the string needs "'"
	}
	else // not a string
	{
		str += value;
	}
	return str; //the string as a field
}
