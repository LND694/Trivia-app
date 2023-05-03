#pragma once

#include "json.hpp"
#include "Response.h"
#include "global.h"

#define SINGEL_DIGIT 1

class JsonResponsePacketSerializer
{
public:
	static Buffer& serializeResponse(const ErrorResponse& errResp);
	static Buffer& serializeResponse(const LoginResponse& logResp);
	static Buffer& serializeResponse(const SignUpResponse& signUpResp);

private:
	static string getPaddedNumber(const int num, const int digits);
	static void addStringToBuffer(Buffer* buf, string str);
};