#pragma once

#include "json.hpp"
#include "Response.h"
#include "global.h"
#include "Singleton.h"

using NLOHMANN_JSON_NAMESPACE::json;

#define ZERO_CHAR '0'
#define SEPERATOR ", "

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

protected:
	JsonResponsePacketSerializer() = default;
	//Singleton fields
	static JsonResponsePacketSerializer* m_instance;
	static Lock m_lock;
private:
	//Help functions
	static string getPaddedNumber(const int num, const int digits);
	static string echoFormat(const string str);
	static Buffer* makeBuffer(const RESPONSE_CODES code, string responseData);
	static void addStringToBuffer(Buffer* buf, string str);

	static string getPlayersString(const vector<string>& players);
	static string getStatisticsString(const vector<string>& statistics);
	static string getRoomDataString(const RoomData& roomData);


	template <class T>
	static string getField(const string nameField, const T value);
};

template<class T>
inline string JsonResponsePacketSerializer::getField(const string nameField, const T value)
{ 
	string str =  "'" + nameField + "':";
	string valueStr = to_string(value);

	if (typeid(T).name() == typeid(string).name()) // the value is a string
	{
		str += "'" + valueStr + "'";
	}
	else
	{
		str += valueStr;
	}
	return str;
}
