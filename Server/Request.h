#pragma once
#include <string>
using std::string;


/// <summary>
/// struct for logins
/// </summary>
typedef struct LoginRequest {
	string username;
	string password;
};

/// <summary>
/// struct for sign up
/// </summary>
typedef struct SignupRequest : LoginRequest {
	string email;

}SingSignupRequest;