#pragma once

#include "json.hpp"
#include "Response.h"
#include "global.h"
#include "Singleton.h"

using NLOHMANN_JSON_NAMESPACE::json;

#define ZERO_CHAR '0'

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

protected:
	JsonResponsePacketSerializer() = default;
	//Singleton fields
	static JsonResponsePacketSerializer* m_instance;
	static Lock m_lock;
private:
	//Help functions
	static string getPaddedNumber(const int num, const int digits);
	static void addStringToBuffer(Buffer* buf, string str);
};