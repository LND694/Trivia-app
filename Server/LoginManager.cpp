#include "LoginManager.h"

/// <summary>
/// signUp in the databse
/// </summary>
/// <param name="email"> the email of the request</param>
/// <param name="password"> the password in the request</param>
/// <param name="username"> the username in the request</param>
/// <returns>SignUp request after register in the database</returns>
SignupRequest& LoginManager::signUp(const string email, const string password, const string username)
{
    SignupRequest* signUpReq = new SignupRequest();
    signUpReq->email = email;
    signUpReq->password = password;
    signUpReq->username = username;
    return* signUpReq;
}


/// <summary>
/// login in the databse
/// </summary>
/// <param name="password"> the password in the request</param>
/// <param name="username"> the username in the request</param>
/// <returns>login request after register in the database</returns>
LoginRequest& LoginManager::login(const string username, const string password)
{
    LoginRequest* logReq = new LoginRequest();
    logReq->username = username;
    logReq->password = password;
    return* logReq;
}
