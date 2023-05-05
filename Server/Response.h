#pragma once
#include <iostream>

constexpr int OK_STATUS_CODE = 1;
const std::string ERROR_MSG = "ERROR";

class Response
{

};

struct ResponseWithStatus
{
	unsigned int status;
};

typedef struct LoginResponse : ResponseWithStatus
{

} LoginResponse;

typedef struct SignUpResopnse : ResponseWithStatus
{

} SignUpResponse;

typedef struct ErrorResopnse : Response
{
	std::string message;
} ErrorResponse;