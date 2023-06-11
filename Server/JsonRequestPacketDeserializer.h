#pragma once
#include "global.h"
#include "Request.h"
#include "json.hpp"
#include "Singleton.h"
using  NLOHMANN_JSON_NAMESPACE::json;

constexpr char SPACE = ' ';
constexpr char NEW_LINE = '\n';
constexpr char END_STR_SYMBOL = '\0';

class JsonRequestPacketDeserializer : public Singleton
{
public:
	static JsonRequestPacketDeserializer* getInstance();
	JsonRequestPacketDeserializer(JsonRequestPacketDeserializer& other) = delete;
	void operator=(const JsonRequestPacketDeserializer& other) = delete;

	//Functions
	static LoginRequest& deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest& desrializeSignupRequest(const Buffer& buffer);
	static GetPlayersInRoomRequest& desrializeGetPlayersRequest(const Buffer& buffer);
	static JoinRoomRequest& desrializeJoinRoomRequest(const Buffer& buffer);
	static CreateRoomRequest& desrializeCreateRoomRequest(const Buffer& buffer);
	static SubmitAnswerRequest& derializeSubmitAnswerRequest(const Buffer& buffer);
protected:
	JsonRequestPacketDeserializer() = default;
	//Singleton fields
	static JsonRequestPacketDeserializer* m_instance;
	static Lock m_lock;
private:
	//Help functions
	static Buffer* getDataFromBuffer(const Buffer& buf);
};
