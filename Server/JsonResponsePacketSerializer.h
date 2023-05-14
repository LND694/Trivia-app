#pragma once

#include "json.hpp"
#include "Response.h"
#include "global.h"

using NLOHMANN_JSON_NAMESPACE::json;

#define ZERO_CHAR '0'

class JsonResponsePacketSerializer
{
public:
	//Functions
	static Buffer& serializeResponse(const ErrorResponse& errResp);
	static Buffer& serializeResponse(const LoginResponse& logResp);
	static Buffer& serializeResponse(const SignUpResponse& signUpResp);

private:
	//Help functions
	static string getPaddedNumber(const int num, const int digits);
	static void addStringToBuffer(Buffer* buf, string str);
};