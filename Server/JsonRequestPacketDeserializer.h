#pragma once
#include "global.h"
#include "Request.h"
#include "json.hpp"
using  NLOHMANN_JSON_NAMESPACE::json;

constexpr char SPACE = ' ';
constexpr char NEW_LINE = '\n';
constexpr char END_STR_SYMBOL = '\0';

class JsonRequestPacketDeserializer
{
public:
	//Functions
	static LoginRequest& deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest& desrializeSignupRequest(const Buffer& buffer);
private:
	//Help functions
	static Buffer* getDataFromBuffer(const Buffer& buf);
};
