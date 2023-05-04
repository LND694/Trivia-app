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
	unsigned int status;
} LoginResponse;

typedef struct SignUpResopnse : ResponseWithStatus
{
	unsigned int status;
} SignUpResponse;

typedef struct ErrorResopnse : Response
{
	std::string message;
} ErrorResponse;