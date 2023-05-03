#pragma once
#include <iostream>

using std::string;

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
	string message;
} ErrorResponse;