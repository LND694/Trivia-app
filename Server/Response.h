#pragma once
#include <iostream>

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