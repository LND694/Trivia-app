#pragma once
#include <iostream>

constexpr int OK_STATUS_CODE = 1;
const std::string ERROR_MSG = "ERROR";

//Base class
class Response
{

};

//Base struct
struct ResponseWithStatus : Response
{
	unsigned int status;
};

//Response structs
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