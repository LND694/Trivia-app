#pragma once
#include <iostream>

class Response
{

};

typedef struct LoginResponse : Response
{
	unsigned int status;
} LoginResponse;

typedef struct SignUpResopnse : Response
{
	unsigned int status;
} SignUpResponse;

typedef struct ErrorResopnse : Response
{
	std::string message;
} ErrorResponse;