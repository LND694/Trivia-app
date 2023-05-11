#include "LoginManager.h"

SignupRequest& LoginManager::signUp(const string email, const string password, const string username)
{
    SignupRequest* signUpReq = new SignupRequest();
    signUpReq->email = email;
    signUpReq->password = password;
    signUpReq->username = username;
    return* signUpReq;
}

LoginRequest& LoginManager::login(const string username, const string password)
{
    LoginRequest* logReq = new LoginRequest();
    logReq->username = username;
    logReq->password = password;
    return* logReq;
}
