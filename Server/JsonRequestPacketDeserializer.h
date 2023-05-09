#pragma once
#include "global.h"
#include "Request.h"
#include "json.hpp"
using  NLOHMANN_JSON_NAMESPACE::json;
class JsonRequestPacketDeserializer
{
public:
	static LoginRequest& deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest& desrializeSignupRequest(const Buffer& buffer);
private:
	static Buffer* getDataFromBuffer(const Buffer& buf);
};
