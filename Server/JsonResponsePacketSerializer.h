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
	static Buffer& serializeResponse(const SignUpResponse& signResp);

private:
	static void addNumToBuffer(Buffer* buf, const int num);
	static int getAmountDigits(const int num);
};