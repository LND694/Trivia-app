#pragma once

#include <ctime>
#include "global.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
using std::time_t;

class IRequestHandler; //pre definition

typedef struct RequestInfo
{
	RequestId id{};
	time_t receivalTime{};
	Buffer buffer;
} RequestInfo;

typedef struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler{};
} RequestResult;


class IRequestHandler
{
public:
	//Functions
	virtual bool isRequestRelevent(const RequestInfo& requestInfo) = 0;	
	virtual RequestResult& handleRequest(const RequestInfo& requestInfo) = 0;
};